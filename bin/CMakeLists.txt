add_executable(ctrack
    main.cpp
)

target_link_libraries(ctrack
    ctrack::ctrack_backend
    stdc++fs
)

install(TARGETS ctrack
    RUNTIME DESTINATION bin
)
