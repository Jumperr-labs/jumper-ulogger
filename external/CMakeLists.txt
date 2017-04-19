add_library(Unity STATIC
        Unity/src/unity.c Unity/extras/fixture/src/unity_fixture.c
        )

target_include_directories(Unity PUBLIC
        Unity/src PUBLIC Unity/extras/fixture/src
        )