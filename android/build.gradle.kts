// Top-level build script. Plugin versions are pinned here; subprojects
// just `id("...")` without re-declaring versions.

plugins {
    // AGP 8.13 is the first AGP line that fully supports Gradle 9.x, which
    // brew ships by default. If you pin an older Gradle in the wrapper, this
    // can drop to 8.7.x.
    id("com.android.application") version "8.13.0" apply false
}
