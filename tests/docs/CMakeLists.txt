# The code shown on the documentation site (docs/*.md include it with --8<--). One test executable: the snippets
# are compiled with hf-core's strict warnings and run against the host mocks.
add_executable(test_doc_snippets
    quickstart.cpp
    threading.cpp
    canopen.cpp
    host_test.cpp
    doc_snippets_test.cpp
)
target_link_libraries(test_doc_snippets PRIVATE hf_core_host_mocks hf_core_host_test_main)
target_compile_options(test_doc_snippets PRIVATE ${HF_CORE_STRICT_WARNINGS})
add_test(NAME test_doc_snippets COMMAND test_doc_snippets)
set_tests_properties(test_doc_snippets PROPERTIES TIMEOUT 60)
