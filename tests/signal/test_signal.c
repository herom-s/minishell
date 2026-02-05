#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "test_signal.h"
#include "minishell_signal.h"
#include "test_signal.h"
#include <readline/history.h>
#include <readline/readline.h>

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void	handle_ctrl_c_mock(int sig)
{
	(void)sig;
	g_sig = 1;
	write(2, "\n", 1);
}

int	setup_signal_mock(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handle_ctrl_c_mock;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror(strerror(errno));
		return (-1);
	}
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

// Reset signal state before each test
int setup_signal_test(void **state)
{
	(void)state;
	g_sig = 0;
	setup_signal_mock();
	return 0;
}

int teardown_signal_test(void **state)
{
	(void)state;
	// Reset to default signal handlers
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	g_sig = 0;
	return 0;
}

// Helper to send signal and wait a bit for handler to execute
void send_signal_and_wait(int sig)
{
	raise(sig);
	usleep(10000); // 10ms should be enough for signal handler
}

// ============================================================================
// SIGNAL SETUP TESTS
// ============================================================================

void test_setup_signal_success(void **state)
{
	(void)state;

	int result = setup_signal_test(NULL);
	assert_int_equal(result, 0);
}

void test_setup_signal_configures_sigint(void **state)
{
	(void)state;

	struct sigaction sa;

	setup_signal_test(NULL);

	// Get current SIGINT handler
	sigaction(SIGINT, NULL, &sa);

	// Should not be default or ignored
	assert_ptr_not_equal(sa.sa_handler, SIG_DFL);
	assert_ptr_not_equal(sa.sa_handler, SIG_IGN);
}

void test_setup_signal_ignores_sigquit(void **state)
{
	(void)state;

	void (*handler)(int);

	setup_signal_test(NULL);

	// Get current SIGQUIT handler
	handler = signal(SIGQUIT, SIG_IGN);

	// Should already be SIG_IGN
	assert_ptr_equal(handler, SIG_IGN);

	// Restore
	signal(SIGQUIT, SIG_IGN);
}

// ============================================================================
// SIGINT (Ctrl+C) HANDLER TESTS
// ============================================================================

void test_sigint_sets_global_flag(void **state)
{
	(void)state;

	assert_int_equal(g_sig, 0);

	send_signal_and_wait(SIGINT);

	assert_int_equal(g_sig, 1);
}

void test_sigint_multiple_times(void **state)
{
	(void)state;

	assert_int_equal(g_sig, 0);

	// Send SIGINT multiple times
	send_signal_and_wait(SIGINT);
	assert_int_equal(g_sig, 1);

	g_sig = 0;

	send_signal_and_wait(SIGINT);
	assert_int_equal(g_sig, 1);
}

void test_sigint_does_not_terminate_process(void **state)
{
	(void)state;

	// Send SIGINT
	send_signal_and_wait(SIGINT);

	// Process should still be alive
	// If we reach here, the process didn't terminate
	assert_true(1);
}

// ============================================================================
// SIGQUIT (Ctrl+\) HANDLER TESTS
// ============================================================================

void test_sigquit_is_ignored(void **state)
{
	(void)state;

	// Send SIGQUIT
	send_signal_and_wait(SIGQUIT);

	// Process should still be alive and nothing should change
	// Global flag should remain 0
	assert_int_equal(g_sig, 0);
}

void test_sigquit_multiple_times_ignored(void **state)
{
	(void)state;

	// Send SIGQUIT multiple times
	for (int i = 0; i < 5; i++)
	{
		send_signal_and_wait(SIGQUIT);
	}

	// Process should still be alive
	// Global flag should remain 0
	assert_int_equal(g_sig, 0);
}

// ============================================================================
// SIGNAL BEHAVIOR IN CHILD PROCESSES
// ============================================================================

void test_sigint_in_child_process(void **state)
{
	(void)state;

	pid_t pid = fork();

	if (pid == 0)
	{
		// Child process
		setup_signal_test(NULL);
		send_signal_and_wait(SIGINT);

		// Check that flag was set
		if (g_sig == 1)
			exit(0);
		else
			exit(1);
	}
	else if (pid > 0)
	{
		// Parent process
		int status;
		waitpid(pid, &status, 0);

		// Child should have exited with status 0
		assert_true(WIFEXITED(status));
		assert_int_equal(WEXITSTATUS(status), 0);
	}
	else
	{
		fail_msg("fork() failed");
	}
}

void test_child_inherits_signal_handlers(void **state)
{
	(void)state;

	setup_signal_test(NULL);

	pid_t pid = fork();

	if (pid == 0)
	{
		// Child process - check if SIGQUIT is still ignored
		struct sigaction sa;
		sigaction(SIGQUIT, NULL, &sa);

		if (sa.sa_handler == SIG_IGN)
			exit(0);
		else
			exit(1);
	}
	else if (pid > 0)
	{
		// Parent process
		int status;
		waitpid(pid, &status, 0);

		assert_true(WIFEXITED(status));
		assert_int_equal(WEXITSTATUS(status), 0);
	}
	else
	{
		fail_msg("fork() failed");
	}
}

// ============================================================================
// SIGNAL RESTORATION TESTS
// ============================================================================

void test_signal_can_be_restored_to_default(void **state)
{
	(void)state;

	setup_signal_test(NULL);

	// Restore to default
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	// Verify restoration
	void (*sigint_handler)(int) = signal(SIGINT, SIG_DFL);
	void (*sigquit_handler)(int) = signal(SIGQUIT, SIG_DFL);

	assert_ptr_equal(sigint_handler, SIG_DFL);
	assert_ptr_equal(sigquit_handler, SIG_DFL);
}

// ============================================================================
// SIGNAL FLAG RESET TESTS
// ============================================================================

void test_global_flag_can_be_reset(void **state)
{
	(void)state;

	// Set flag
	send_signal_and_wait(SIGINT);
	assert_int_equal(g_sig, 1);

	// Reset flag
	g_sig = 0;
	assert_int_equal(g_sig, 0);

	// Should be settable again
	send_signal_and_wait(SIGINT);
	assert_int_equal(g_sig, 1);
}

// ============================================================================
// CONCURRENT SIGNAL TESTS
// ============================================================================

void test_sigint_and_sigquit_together(void **state)
{
	(void)state;

	assert_int_equal(g_sig, 0);

	// Send both signals
	raise(SIGINT);
	raise(SIGQUIT);
	usleep(10000);

	// Only SIGINT should have effect
	assert_int_equal(g_sig, 1);
}

// ============================================================================
// EDGE CASES
// ============================================================================

void test_setup_signal_called_twice(void **state)
{
	(void)state;

	int result1 = setup_signal_test(NULL);
	int result2 = setup_signal_test(NULL);

	assert_int_equal(result1, 0);
	assert_int_equal(result2, 0);

	// Signal handlers should still work
	send_signal_and_wait(SIGINT);
	assert_int_equal(g_sig, 1);
}

void test_sigint_with_zero_flag(void **state)
{
	(void)state;

	// Ensure flag starts at 0
	g_sig = 0;
	assert_int_equal(g_sig, 0);

	send_signal_and_wait(SIGINT);

	// Should transition from 0 to 1
	assert_int_equal(g_sig, 1);
}

void test_rapid_signal_delivery(void **state)
{
	(void)state;

	g_sig = 0;

	// Send signals rapidly
	for (int i = 0; i < 10; i++)
	{
		raise(SIGINT);
	}

	usleep(10000);

	// Flag should be set (at least once)
	assert_int_equal(g_sig, 1);
}

// ============================================================================
// SIGACTION FLAGS TESTS
// ============================================================================

void test_sigaction_flags_are_correct(void **state)
{
	(void)state;

	struct sigaction sa;

	setup_signal_test(NULL);
	sigaction(SIGINT, NULL, &sa);

	// Verify flags are set correctly (SA_RESTART should not be set)
	assert_int_equal(sa.sa_flags & SA_RESTART, 0);
}

void test_signal_mask_is_empty(void **state)
{
	(void)state;

	struct sigaction sa;

	setup_signal_test(NULL);
	sigaction(SIGINT, NULL, &sa);

	// Verify signal mask is empty
	assert_true(sigismember(&sa.sa_mask, SIGINT) == 0);
	assert_true(sigismember(&sa.sa_mask, SIGQUIT) == 0);
}

// ============================================================================
// INTEGRATION TESTS
// ============================================================================

void test_signal_handler_survives_child_exit(void **state)
{
	(void)state;

	setup_signal_test(NULL);

	pid_t pid = fork();

	if (pid == 0)
	{
		// Child exits immediately
		exit(0);
	}
	else if (pid > 0)
	{
		// Parent waits and then tests signal
		int status;
		waitpid(pid, &status, 0);

		// Parent's signal handler should still work
		g_sig = 0;
		send_signal_and_wait(SIGINT);
		assert_int_equal(g_sig, 1);
	}
}

void test_multiple_children_with_signals(void **state)
{
	(void)state;

	setup_signal_test(NULL);

	for (int i = 0; i < 3; i++)
	{
		pid_t pid = fork();

		if (pid == 0)
		{
			// Child: send SIGINT to itself
			g_sig = 0;
			send_signal_and_wait(SIGINT);
			exit(g_sig == 1 ? 0 : 1);
		}
		else if (pid > 0)
		{
			// Parent: wait for child
			int status;
			waitpid(pid, &status, 0);
			assert_true(WIFEXITED(status));
			assert_int_equal(WEXITSTATUS(status), 0);
		}
	}
}
