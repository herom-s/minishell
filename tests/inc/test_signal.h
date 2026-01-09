#ifndef TEST_SIGNAL_H
# define TEST_SIGNAL_H

// Setup and teardown
int setup_signal_test(void **state);
int teardown_signal_test(void **state);

// Signal setup tests
void test_setup_signal_success(void **state);
void test_setup_signal_configures_sigint(void **state);
void test_setup_signal_ignores_sigquit(void **state);

// SIGINT handler tests
void test_sigint_sets_global_flag(void **state);
void test_sigint_multiple_times(void **state);
void test_sigint_does_not_terminate_process(void **state);

// SIGQUIT handler tests
void test_sigquit_is_ignored(void **state);
void test_sigquit_multiple_times_ignored(void **state);

// Child process tests
void test_sigint_in_child_process(void **state);
void test_child_inherits_signal_handlers(void **state);

// Signal restoration tests
void test_signal_can_be_restored_to_default(void **state);

// Flag reset tests
void test_global_flag_can_be_reset(void **state);

// Concurrent signal tests
void test_sigint_and_sigquit_together(void **state);

// Edge case tests
void test_setup_signal_called_twice(void **state);
void test_sigint_with_zero_flag(void **state);
void test_rapid_signal_delivery(void **state);

// Sigaction tests
void test_sigaction_flags_are_correct(void **state);
void test_signal_mask_is_empty(void **state);

// Integration tests
void test_signal_handler_survives_child_exit(void **state);
void test_multiple_children_with_signals(void **state);

#endif
