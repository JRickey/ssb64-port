// settings.gradle.kts — root settings for the Android Gradle build.

pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

dependencyResolutionManagement {
    // Force-fail any subproject that tries to declare its own repos. Single
    // source of truth for where we pull AGP / AndroidX from.
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "BattleShip"
include(":app")
