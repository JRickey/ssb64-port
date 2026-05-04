# raphnet_get_version_retry — Open() boot fails at GET_VERSION (v0.7.4-beta)

**Status:** RESOLVED
**Component:** `libultraship/src/ship/controller/raphnet/RaphnetTransport.cpp`
**Hardware:** raphnet-tech GameCube/N64-to-USB v3 (vid=0x289b pid=0x0061), Windows.

## Symptom

Native raphnet path enumerates the adapter, opens it, sizes the report buffer
correctly via `HidP_GetCaps`, sends the defensive `SUSPEND_POLLING(0)` and gets
back a valid 8-byte response (`rep[0]=0x03`, opcode echo confirmed). Then the
very next request — `GET_VERSION (0x04)` — returns `n=0 err=Success` and Open()
bails. Adapter is skipped on every retry. Repro is deterministic across replug,
restart, and multiple Open attempts in the same session.

```
[raphnet-diag] defensive SUSPEND_POLLING(0): n=8 rep[0]=0x03 rep_bytes='03 00 00 00 00 00 00 00'
[raphnet] GET_VERSION exchange failed (n=0, err=Success)
[raphnet] GET_VERSION failed on '\\?\HID#VID_289B&PID_0061&MI_01...' — closing
```

## Root cause

`RaphnetTransport::Exchange`'s receive loop early-exited as soon as
`hid_get_feature_report` returned `got > 0`, then computed payload as
`payloadLen = got - 1` and returned that. On Windows hidapi's
`hid_get_feature_report` always adds 1 to `bytes_returned` to account for the
report-id byte (`libultraship/_deps/hidapi/windows/hid.c::hid_get_feature_report`),
so a "device returned no payload" read surfaces as `got == 1` — which we
happily treated as "successful 0-byte read" and bailed with `n=0`. On
Linux/macOS the same condition surfaces as `got == 0` and we did retry it.

`SUSPEND_POLLING(0)` happens to ack quickly — by the time we issue the GET the
device has already populated its response feature report, so the first read
returns 8 bytes and the early-exit branch produces a real payload.
`GET_VERSION` takes a hair longer; the device hasn't populated the response by
the time we issue the GET, the first read returns just the report-id byte, and
we give up before retrying.

raphnet's reference `gcn64tools/src/rntlib/raphnetadapter.c::rnt_exchange` polls
`HidD_GetFeature` in a loop until `res_len > 0`, with a ~1000 ms ceiling. We
already do the loop — but only on the Linux/macOS-shaped sentinel
(`got == 0`), not the Windows-shaped one (`got == 1`).

## Fix

Change the early-exit condition from `if (got > 0)` to `if (got > 1)` so a
zero-payload read on either platform falls through to the retry path. Also log
`last_got` when the deadline trips so a future repro shows whether retries
were getting `0` or `1`.

```c++
if (got > 1) {                       // had to be > 1, not > 0
    int payloadLen = got - 1;
    size_t copyOut = std::min(static_cast<size_t>(payloadLen), rxMax);
    std::memcpy(rx, recvBuf.data() + 1, copyOut);
    return static_cast<int>(copyOut);
}
if (got < 0) { ... return -1; }
// got == 0 (Linux/macOS) or got == 1 (Windows hidapi report-id+1
// accounting): device hasn't produced a response yet. Sleep briefly
// and retry until deadline.
```

Single-file change in `RaphnetTransport.cpp::Exchange`. No protocol or wire-
format change. The same Exchange path is used by SUSPEND_POLLING(1), the
per-frame Poll, GetControllerType, and SetVibration, so the fix applies to all
of them automatically.

## Audit hook

Any new caller of `hid_get_feature_report` in the codebase needs the same
"`got == 1` is a no-payload sentinel on Windows" awareness. The hidapi convention
of inclusive-of-report-id byte counts trips up naive byte-count checks; treat
"actual payload bytes" (`got - 1`) as the meaningful quantity, not `got`
itself.
