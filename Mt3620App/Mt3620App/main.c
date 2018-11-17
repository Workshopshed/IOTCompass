#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// applibs_versions.h defines the API struct versions to use for applibs APIs.
#include "applibs_versions.h"
#include <applibs/log.h>

#include "mt3620_rdb.h"
#include "stepper.h"

// This C application for the MT3620 Reference Development Board (Azure Sphere)
// outputs a string every second to Visual Studio's Device Output window
//
// It uses the API for the following Azure Sphere application libraries:
// - log (messages shown in Visual Studio's Device Output window during debugging)

static volatile sig_atomic_t terminationRequired = false;

//volatile as could be changed by timer.
static volatile stepper_t stepper = { .gpio1Fd = -1,
									.pin1 = MT3620_RDB_HEADER1_PIN4_GPIO,
									.gpio2Fd = -1,
									.pin2 = MT3620_RDB_HEADER1_PIN6_GPIO,
									.gpio3Fd = -1,
									.pin3 = MT3620_RDB_HEADER1_PIN8_GPIO,
									.gpio4Fd = -1,
									.pin4 = MT3620_RDB_HEADER1_PIN10_GPIO,
									.direction = STOP };

/// <summary>
///     Signal handler for termination requests. This handler must be async-signal-safe.
/// </summary>
static void TerminationHandler(int signalNumber)
{
    // Don't use Log_Debug here, as it is not guaranteed to be async-signal-safe.
    terminationRequired = true;
}

static int InitPeripheralsAndHandlers(void)
{
	// Register a SIGTERM handler for termination requests
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = TerminationHandler;
	sigaction(SIGTERM, &action, NULL);

	//Init Hardware
	STEPPER_Init(&stepper);
	return 0;
}

/// <summary>
///     Close peripherals and handlers.
/// </summary>
static void ClosePeripheralsAndHandlers(void) {
	Log_Debug("Closing file descriptors\n");

	STEPPER_Terminate(&stepper);
}


/// <summary>
///     Main entry point for this sample.
/// </summary>
int main(int argc, char *argv[])
{
	Log_Debug("Application starting\n");

	if (InitPeripheralsAndHandlers() != 0) {
		terminationRequired = true;
	}

	STEPPER_SetSteps(&stepper, 10000);

	// Main loop
	const struct timespec sleepTime = { 0, 5000000 }; //5ms
	
	while (!terminationRequired) {
		STEPPER_Run(&stepper);
		Log_Debug(".");
		nanosleep(&sleepTime, NULL);
	}

	ClosePeripheralsAndHandlers();
	Log_Debug("Application exiting\n");

	return 0;
}
