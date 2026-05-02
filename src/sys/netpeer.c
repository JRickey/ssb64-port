#include <sys/netpeer.h>

#include <sys/netinput.h>

#ifdef PORT
extern char *getenv(const char *name);
extern int atoi(const char *s);
extern void port_log(const char *fmt, ...);
#endif

#if defined(PORT) && !defined(_WIN32)
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#define SYNETPEER_MAGIC 0x53534E50 // SSNP
#define SYNETPEER_VERSION 1
#define SYNETPEER_MAX_PACKET_FRAMES 4
#define SYNETPEER_FRAME_BYTES 8
#define SYNETPEER_PACKET_BYTES (4 + 2 + 2 + 4 + 4 + 1 + 1 + 1 + 1 + (SYNETPEER_MAX_PACKET_FRAMES * SYNETPEER_FRAME_BYTES) + 4)
#define SYNETPEER_DEFAULT_INPUT_DELAY 2
#define SYNETPEER_DEFAULT_SESSION_ID 1
#define SYNETPEER_LOG_INTERVAL 120

typedef struct SYNetPeerPacketFrame
{
	u32 tick;
	u16 buttons;
	s8 stick_x;
	s8 stick_y;

} SYNetPeerPacketFrame;

sb32 sSYNetPeerIsEnabled;
sb32 sSYNetPeerIsConfigured;
sb32 sSYNetPeerIsActive;
s32 sSYNetPeerLocalPlayer;
s32 sSYNetPeerRemotePlayer;
u32 sSYNetPeerInputDelay;
u32 sSYNetPeerSessionID;
u32 sSYNetPeerHighestRemoteTick;
u32 sSYNetPeerPacketsSent;
u32 sSYNetPeerPacketsReceived;
u32 sSYNetPeerPacketsDropped;
u32 sSYNetPeerFramesStaged;
u32 sSYNetPeerLateFrames;
u32 sSYNetPeerInputChecksum;
u32 sSYNetPeerLastLogTick;

#if defined(PORT) && !defined(_WIN32)
s32 sSYNetPeerSocket = -1;
struct sockaddr_in sSYNetPeerBindAddress;
struct sockaddr_in sSYNetPeerPeerAddress;
#endif

u32 syNetPeerChecksumAccumulateU32(u32 checksum, u32 value)
{
	checksum ^= value;
	checksum *= 16777619U;

	return checksum;
}

u32 syNetPeerChecksumAccumulateFrame(u32 checksum, const SYNetPeerPacketFrame *frame)
{
	checksum = syNetPeerChecksumAccumulateU32(checksum, frame->tick);
	checksum = syNetPeerChecksumAccumulateU32(checksum, frame->buttons);
	checksum = syNetPeerChecksumAccumulateU32(checksum, (u8)frame->stick_x);
	checksum = syNetPeerChecksumAccumulateU32(checksum, (u8)frame->stick_y);

	return checksum;
}

u32 syNetPeerChecksumPacket(u32 session_id, u32 ack_tick, u8 player, u8 frame_count, const SYNetPeerPacketFrame *frames)
{
	u32 checksum = 2166136261U;
	s32 i;

	checksum = syNetPeerChecksumAccumulateU32(checksum, SYNETPEER_MAGIC);
	checksum = syNetPeerChecksumAccumulateU32(checksum, SYNETPEER_VERSION);
	checksum = syNetPeerChecksumAccumulateU32(checksum, session_id);
	checksum = syNetPeerChecksumAccumulateU32(checksum, ack_tick);
	checksum = syNetPeerChecksumAccumulateU32(checksum, player);
	checksum = syNetPeerChecksumAccumulateU32(checksum, frame_count);

	for (i = 0; i < frame_count; i++)
	{
		checksum = syNetPeerChecksumAccumulateFrame(checksum, &frames[i]);
	}
	return checksum;
}

void syNetPeerWriteU8(u8 **cursor, u8 value)
{
	*(*cursor)++ = value;
}

void syNetPeerWriteU16(u8 **cursor, u16 value)
{
	*(*cursor)++ = (value >> 8) & 0xFF;
	*(*cursor)++ = value & 0xFF;
}

void syNetPeerWriteU32(u8 **cursor, u32 value)
{
	*(*cursor)++ = (value >> 24) & 0xFF;
	*(*cursor)++ = (value >> 16) & 0xFF;
	*(*cursor)++ = (value >> 8) & 0xFF;
	*(*cursor)++ = value & 0xFF;
}

u8 syNetPeerReadU8(const u8 **cursor)
{
	return *(*cursor)++;
}

u16 syNetPeerReadU16(const u8 **cursor)
{
	u16 value = (u16)(*(*cursor)++) << 8;

	value |= *(*cursor)++;

	return value;
}

u32 syNetPeerReadU32(const u8 **cursor)
{
	u32 value = (u32)(*(*cursor)++) << 24;

	value |= (u32)(*(*cursor)++) << 16;
	value |= (u32)(*(*cursor)++) << 8;
	value |= *(*cursor)++;

	return value;
}

#if defined(PORT) && !defined(_WIN32)
const char *syNetPeerFindPortSeparator(const char *text)
{
	const char *separator = NULL;

	while (*text != '\0')
	{
		if (*text == ':')
		{
			separator = text;
		}
		text++;
	}
	return separator;
}

sb32 syNetPeerStringEquals(const char *a, const char *b)
{
	while ((*a != '\0') && (*b != '\0'))
	{
		if (*a != *b)
		{
			return FALSE;
		}
		a++;
		b++;
	}
	return (*a == *b) ? TRUE : FALSE;
}

sb32 syNetPeerParseIPv4Address(const char *text, struct sockaddr_in *out_address)
{
	const char *colon;
	s32 host_length;
	s32 port;
	char host[64];

	if ((text == NULL) || (out_address == NULL))
	{
		return FALSE;
	}
	colon = syNetPeerFindPortSeparator(text);

	if ((colon == NULL) || (colon == text) || (*(colon + 1) == '\0'))
	{
		return FALSE;
	}
	host_length = colon - text;

	if (host_length >= (s32)sizeof(host))
	{
		return FALSE;
	}
	memcpy(host, text, host_length);
	host[host_length] = '\0';

	port = atoi(colon + 1);

	if ((port <= 0) || (port > 65535))
	{
		return FALSE;
	}
	memset(out_address, 0, sizeof(*out_address));
	out_address->sin_family = AF_INET;
	out_address->sin_port = htons((u16)port);

	if ((syNetPeerStringEquals(host, "*") != FALSE) || (syNetPeerStringEquals(host, "0.0.0.0") != FALSE))
	{
		out_address->sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else if (inet_pton(AF_INET, host, &out_address->sin_addr) != 1)
	{
		return FALSE;
	}
	return TRUE;
}

void syNetPeerCloseSocket(void)
{
	if (sSYNetPeerSocket >= 0)
	{
		close(sSYNetPeerSocket);
		sSYNetPeerSocket = -1;
	}
}
#endif

void syNetPeerInitDebugEnv(void)
{
#ifdef PORT
	char *netplay_env = getenv("SSB64_NETPLAY");
	char *local_player_env;
	char *remote_player_env;
	char *delay_env;
	char *session_env;
	char *bind_env;
	char *peer_env;

	sSYNetPeerIsEnabled = FALSE;
	sSYNetPeerIsConfigured = FALSE;
	sSYNetPeerLocalPlayer = 0;
	sSYNetPeerRemotePlayer = 1;
	sSYNetPeerInputDelay = SYNETPEER_DEFAULT_INPUT_DELAY;
	sSYNetPeerSessionID = SYNETPEER_DEFAULT_SESSION_ID;

	if ((netplay_env == NULL) || (atoi(netplay_env) == 0))
	{
		return;
	}
	sSYNetPeerIsEnabled = TRUE;

	local_player_env = getenv("SSB64_NETPLAY_LOCAL_PLAYER");
	remote_player_env = getenv("SSB64_NETPLAY_REMOTE_PLAYER");
	delay_env = getenv("SSB64_NETPLAY_DELAY");
	session_env = getenv("SSB64_NETPLAY_SESSION");
	bind_env = getenv("SSB64_NETPLAY_BIND");
	peer_env = getenv("SSB64_NETPLAY_PEER");

	if (local_player_env != NULL)
	{
		sSYNetPeerLocalPlayer = atoi(local_player_env);
	}
	if (remote_player_env != NULL)
	{
		sSYNetPeerRemotePlayer = atoi(remote_player_env);
	}
	if (delay_env != NULL)
	{
		s32 delay = atoi(delay_env);

		if (delay >= 0)
		{
			sSYNetPeerInputDelay = delay;
		}
	}
	if (session_env != NULL)
	{
		s32 session_id = atoi(session_env);

		if (session_id > 0)
		{
			sSYNetPeerSessionID = session_id;
		}
	}
	if ((sSYNetPeerLocalPlayer < 0) || (sSYNetPeerLocalPlayer >= MAXCONTROLLERS) ||
		(sSYNetPeerRemotePlayer < 0) || (sSYNetPeerRemotePlayer >= MAXCONTROLLERS) ||
		(sSYNetPeerLocalPlayer == sSYNetPeerRemotePlayer))
	{
		port_log("SSB64 NetPeer: invalid players local=%d remote=%d\n",
		         sSYNetPeerLocalPlayer, sSYNetPeerRemotePlayer);
		return;
	}
#if !defined(_WIN32)
	if ((syNetPeerParseIPv4Address(bind_env, &sSYNetPeerBindAddress) == FALSE) ||
		(syNetPeerParseIPv4Address(peer_env, &sSYNetPeerPeerAddress) == FALSE))
	{
		port_log("SSB64 NetPeer: invalid bind/peer; expected IPv4 host:port\n");
		return;
	}
	sSYNetPeerIsConfigured = TRUE;
	port_log("SSB64 NetPeer: configured local=%d remote=%d delay=%u session=%u bind=%s peer=%s\n",
	         sSYNetPeerLocalPlayer, sSYNetPeerRemotePlayer, sSYNetPeerInputDelay,
	         sSYNetPeerSessionID, bind_env, peer_env);
#else
	port_log("SSB64 NetPeer: debug UDP netplay is not implemented on Windows yet\n");
#endif
#endif
}

void syNetPeerStartVSSession(void)
{
#if defined(PORT) && !defined(_WIN32)
	s32 flags;
	s32 reuse = 1;

	if ((sSYNetPeerIsEnabled == FALSE) || (sSYNetPeerIsConfigured == FALSE))
	{
		return;
	}
	syNetPeerStopVSSession();

	sSYNetPeerSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (sSYNetPeerSocket < 0)
	{
		port_log("SSB64 NetPeer: socket failed errno=%d\n", errno);
		return;
	}
	setsockopt(sSYNetPeerSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	if (bind(sSYNetPeerSocket, (struct sockaddr*)&sSYNetPeerBindAddress, sizeof(sSYNetPeerBindAddress)) != 0)
	{
		port_log("SSB64 NetPeer: bind failed errno=%d\n", errno);
		syNetPeerCloseSocket();
		return;
	}
	flags = fcntl(sSYNetPeerSocket, F_GETFL, 0);

	if ((flags < 0) || (fcntl(sSYNetPeerSocket, F_SETFL, flags | O_NONBLOCK) != 0))
	{
		port_log("SSB64 NetPeer: nonblocking setup failed errno=%d\n", errno);
		syNetPeerCloseSocket();
		return;
	}
	sSYNetPeerHighestRemoteTick = 0;
	sSYNetPeerPacketsSent = 0;
	sSYNetPeerPacketsReceived = 0;
	sSYNetPeerPacketsDropped = 0;
	sSYNetPeerFramesStaged = 0;
	sSYNetPeerLateFrames = 0;
	sSYNetPeerInputChecksum = 2166136261U;
	sSYNetPeerLastLogTick = 0;
	sSYNetPeerIsActive = TRUE;

	syNetInputSetSlotSource(sSYNetPeerLocalPlayer, nSYNetInputSourceLocal);
	syNetInputSetSlotSource(sSYNetPeerRemotePlayer, nSYNetInputSourceRemotePredicted);

	port_log("SSB64 NetPeer: VS session start local=%d remote=%d delay=%u\n",
	         sSYNetPeerLocalPlayer, sSYNetPeerRemotePlayer, sSYNetPeerInputDelay);
#endif
}

void syNetPeerBuildPacket(u8 *buffer, u32 *out_size)
{
	SYNetInputFrame published_frame;
	SYNetInputFrame history_frame;
	SYNetPeerPacketFrame frames[SYNETPEER_MAX_PACKET_FRAMES];
	SYNetPeerPacketFrame zero_frame;
	u8 *cursor = buffer;
	u32 latest_tick;
	u32 checksum;
	s32 frame_count = 0;
	s32 back;
	s32 i;

	memset(frames, 0, sizeof(frames));
	memset(&zero_frame, 0, sizeof(zero_frame));

	if (syNetInputGetPublishedFrame(sSYNetPeerLocalPlayer, &published_frame) == FALSE)
	{
		*out_size = 0;
		return;
	}
	latest_tick = published_frame.tick;

	for (back = SYNETPEER_MAX_PACKET_FRAMES - 1; back >= 0; back--)
	{
		if ((latest_tick >= (u32)back) &&
			(syNetInputGetHistoryFrame(sSYNetPeerLocalPlayer, latest_tick - back, &history_frame) != FALSE))
		{
			frames[frame_count].tick = history_frame.tick + sSYNetPeerInputDelay;
			frames[frame_count].buttons = history_frame.buttons;
			frames[frame_count].stick_x = history_frame.stick_x;
			frames[frame_count].stick_y = history_frame.stick_y;
			frame_count++;
		}
	}
	checksum = syNetPeerChecksumPacket
	(
		sSYNetPeerSessionID,
		sSYNetPeerHighestRemoteTick,
		(u8)sSYNetPeerLocalPlayer,
		(u8)frame_count,
		frames
	);

	syNetPeerWriteU32(&cursor, SYNETPEER_MAGIC);
	syNetPeerWriteU16(&cursor, SYNETPEER_VERSION);
	syNetPeerWriteU16(&cursor, 0);
	syNetPeerWriteU32(&cursor, sSYNetPeerSessionID);
	syNetPeerWriteU32(&cursor, sSYNetPeerHighestRemoteTick);
	syNetPeerWriteU8(&cursor, (u8)sSYNetPeerLocalPlayer);
	syNetPeerWriteU8(&cursor, (u8)frame_count);
	syNetPeerWriteU8(&cursor, (u8)sSYNetPeerLocalPlayer);
	syNetPeerWriteU8(&cursor, (u8)sSYNetPeerRemotePlayer);

	for (i = 0; i < SYNETPEER_MAX_PACKET_FRAMES; i++)
	{
		SYNetPeerPacketFrame *frame = (i < frame_count) ? &frames[i] : &zero_frame;

		syNetPeerWriteU32(&cursor, frame->tick);
		syNetPeerWriteU16(&cursor, frame->buttons);
		syNetPeerWriteU8(&cursor, (u8)frame->stick_x);
		syNetPeerWriteU8(&cursor, (u8)frame->stick_y);
	}
	syNetPeerWriteU32(&cursor, checksum);
	*out_size = SYNETPEER_PACKET_BYTES;
}

void syNetPeerSendLocalInput(void)
{
#if defined(PORT) && !defined(_WIN32)
	u8 buffer[SYNETPEER_PACKET_BYTES];
	u32 size;

	syNetPeerBuildPacket(buffer, &size);

	if (size == 0)
	{
		return;
	}
	if (sendto(sSYNetPeerSocket, buffer, size, 0,
	           (struct sockaddr*)&sSYNetPeerPeerAddress, sizeof(sSYNetPeerPeerAddress)) == (ssize_t)size)
	{
		sSYNetPeerPacketsSent++;
	}
#endif
}

void syNetPeerHandlePacket(const u8 *buffer, s32 size)
{
	const u8 *cursor = buffer;
	SYNetPeerPacketFrame frames[SYNETPEER_MAX_PACKET_FRAMES];
	u32 magic;
	u32 session_id;
	u32 ack_tick;
	u32 checksum;
	u32 expected_checksum;
	u16 version;
	u8 player;
	u8 frame_count;
	u8 packet_local_player;
	u8 packet_remote_player;
	u32 current_tick = syNetInputGetTick();
	s32 i;

	if (size != SYNETPEER_PACKET_BYTES)
	{
		sSYNetPeerPacketsDropped++;
		return;
	}
	memset(frames, 0, sizeof(frames));

	magic = syNetPeerReadU32(&cursor);
	version = syNetPeerReadU16(&cursor);
	(void)syNetPeerReadU16(&cursor);
	session_id = syNetPeerReadU32(&cursor);
	ack_tick = syNetPeerReadU32(&cursor);
	player = syNetPeerReadU8(&cursor);
	frame_count = syNetPeerReadU8(&cursor);
	packet_local_player = syNetPeerReadU8(&cursor);
	packet_remote_player = syNetPeerReadU8(&cursor);

	if ((magic != SYNETPEER_MAGIC) || (version != SYNETPEER_VERSION) ||
		(session_id != sSYNetPeerSessionID) || (player != (u8)sSYNetPeerRemotePlayer) ||
		(packet_local_player != (u8)sSYNetPeerRemotePlayer) ||
		(packet_remote_player != (u8)sSYNetPeerLocalPlayer) ||
		(frame_count > SYNETPEER_MAX_PACKET_FRAMES))
	{
		sSYNetPeerPacketsDropped++;
		return;
	}
	for (i = 0; i < SYNETPEER_MAX_PACKET_FRAMES; i++)
	{
		frames[i].tick = syNetPeerReadU32(&cursor);
		frames[i].buttons = syNetPeerReadU16(&cursor);
		frames[i].stick_x = (s8)syNetPeerReadU8(&cursor);
		frames[i].stick_y = (s8)syNetPeerReadU8(&cursor);
	}
	checksum = syNetPeerReadU32(&cursor);
	expected_checksum = syNetPeerChecksumPacket(session_id, ack_tick, player, frame_count, frames);

	if (checksum != expected_checksum)
	{
		sSYNetPeerPacketsDropped++;
		return;
	}
	sSYNetPeerPacketsReceived++;

	for (i = 0; i < frame_count; i++)
	{
		if (frames[i].tick < current_tick)
		{
			sSYNetPeerLateFrames++;
		}
		if (frames[i].tick > sSYNetPeerHighestRemoteTick)
		{
			sSYNetPeerHighestRemoteTick = frames[i].tick;
		}
		syNetInputSetRemoteInput
		(
			sSYNetPeerRemotePlayer,
			frames[i].tick,
			frames[i].buttons,
			frames[i].stick_x,
			frames[i].stick_y
		);
		sSYNetPeerFramesStaged++;
		sSYNetPeerInputChecksum = syNetPeerChecksumAccumulateU32(sSYNetPeerInputChecksum, sSYNetPeerRemotePlayer);
		sSYNetPeerInputChecksum = syNetPeerChecksumAccumulateFrame(sSYNetPeerInputChecksum, &frames[i]);
	}
}

void syNetPeerReceiveRemoteInput(void)
{
#if defined(PORT) && !defined(_WIN32)
	u8 buffer[SYNETPEER_PACKET_BYTES];

	while (TRUE)
	{
		ssize_t size = recvfrom(sSYNetPeerSocket, buffer, sizeof(buffer), 0, NULL, NULL);

		if (size < 0)
		{
			if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
			{
				sSYNetPeerPacketsDropped++;
			}
			break;
		}
		syNetPeerHandlePacket(buffer, (s32)size);
	}
#endif
}

void syNetPeerLogStats(void)
{
#ifdef PORT
	u32 tick = syNetInputGetTick();

	if ((tick == 0) || ((tick - sSYNetPeerLastLogTick) < SYNETPEER_LOG_INTERVAL))
	{
		return;
	}
	sSYNetPeerLastLogTick = tick;

	port_log("SSB64 NetPeer: tick=%u sent=%u recv=%u dropped=%u staged=%u highest_remote=%u late=%u checksum=0x%08X\n",
	         tick, sSYNetPeerPacketsSent, sSYNetPeerPacketsReceived, sSYNetPeerPacketsDropped,
	         sSYNetPeerFramesStaged, sSYNetPeerHighestRemoteTick, sSYNetPeerLateFrames,
	         sSYNetPeerInputChecksum);
#endif
}

void syNetPeerUpdate(void)
{
	if (sSYNetPeerIsActive == FALSE)
	{
		return;
	}
	syNetPeerReceiveRemoteInput();
	syNetPeerSendLocalInput();
	syNetPeerLogStats();
}

void syNetPeerStopVSSession(void)
{
#if defined(PORT) && !defined(_WIN32)
	if (sSYNetPeerIsActive != FALSE)
	{
		port_log("SSB64 NetPeer: VS session stop sent=%u recv=%u dropped=%u staged=%u late=%u checksum=0x%08X\n",
		         sSYNetPeerPacketsSent, sSYNetPeerPacketsReceived, sSYNetPeerPacketsDropped,
		         sSYNetPeerFramesStaged, sSYNetPeerLateFrames, sSYNetPeerInputChecksum);
	}
	syNetPeerCloseSocket();
#endif
	sSYNetPeerIsActive = FALSE;
}
