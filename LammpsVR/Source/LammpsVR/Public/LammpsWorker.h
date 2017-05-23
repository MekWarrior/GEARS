// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LammpsTypes.h"
#include <mutex>
/**
 * Worker class that runs a LAMMPS iteration on a separate thread. Ensure that the user (LammpsController) can continue 
 * to function, instead of wait for a large LAMMPS computation to finish. 
 */
class LAMMPSVR_API LammpsWorker : FRunnable 
{
public:
	LammpsWorker(void* lammps_, _LammpsCommand commandFunction_, std::mutex* lock_);
	~LammpsWorker();

	void DeployWorker();
	void SetLammpsInstance(void* lammps_, _LammpsCommand commandFunction_);
	void SetSignalLock(std::mutex* lock_);

	/* Runnable Interface (all must be defined, but only run is used) */
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	/* Synchronization */
	static void LockThread();
	static void UnlockThread();

	/* Clean up Functions */
	void EnsureCompletion();
	void SignalCompletion();

private:
	/* Lammps Related */
	void* m_lammps;
	_LammpsCommand m_lammpsCommand;

	/* Multi-Threading Related */
	FRunnableThread* m_thread;              // Instance of the thread that actually run separate from unreal
	std::mutex* m_signalLock;				// Unlocking this signals to the LammpsController that an iteration has finished
	static std::mutex m_internalLock;		// Just in case
};
