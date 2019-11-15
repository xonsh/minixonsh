set(MINIXONSH ${CURRENT_BINARY_DIR}/../minixonsh)

add_test(test1 ${MINIXONSH} test1.xsh)

add_test(test2 ${MINIXONSH} test2.xsh)
set_tests_properties(test2 PROPERTIES WILL_FAIL TRUE)
