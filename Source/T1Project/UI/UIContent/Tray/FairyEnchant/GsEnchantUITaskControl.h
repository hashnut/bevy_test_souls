#pragma once
#include "CoreMinimal.h"

#pragma region FGsTask
class T1PROJECT_API FGsTask
{
public:
	FString _taskName;
	bool _started{ false };
	bool _finished{ false };

public:
	FGsTask() = default;
	FGsTask(FString taskName) : _taskName(taskName) {};
	virtual ~FGsTask() = default;

	virtual bool condStart() { return true; }
	virtual void onStart() {} //Task 시작 시 수행될 작업
	virtual bool cond() { return false; } // Task 종료 조건, default;false
	virtual void onFinish() {} //Task 종료 시 수행될 작업
	virtual void update(float delta) {}

	void finish() { _finished = true; }
};
#pragma endregion

#pragma region FGsTaskManager
class T1PROJECT_API FGsTaskManager
{
public:
	TQueue<TSharedPtr<FGsTask>> _taskQueue;
	TFunction<void()> _taskFinished{ nullptr };

public:
	void pushTask(TSharedPtr<FGsTask> task)
	{
		_taskQueue.Enqueue(task);
	}

	void clearTask(bool instanceClear = false)
	{
		while (!_taskQueue.IsEmpty())
		{
			auto task = *_taskQueue.Peek();
			_taskQueue.Pop();
			if (!instanceClear && task.IsValid() && task->_started)
			{
				task->_finished = true;
				task->onFinish();
			}

			task = nullptr;
		}
	}

	TWeakPtr<FGsTask> runTask() { return (!_taskQueue.IsEmpty() && (*_taskQueue.Peek())->_started) ? *(_taskQueue.Peek()) : nullptr; }

	void doTasks(float delta = 0.f)
	{
		if (!_taskQueue.IsEmpty())
		{
			auto task = *_taskQueue.Peek();
			if (!task->_started && task->condStart())
			{
				task->_started = true;
				task->onStart();
			}

			if (task->_finished || task->cond())
			{
				_taskQueue.Pop();
				task->_finished = true;
				task->onFinish();
			}
			else
			{
				task->update(delta);
			}

			if (_taskQueue.IsEmpty() && _taskFinished)
			{
				_taskFinished();
			}
		}
	}
};

#pragma endregion

template<typename E>
class T1PROJECT_API FGsTaskStateManager
{
public:
	TMap<E, TSharedPtr<FGsTask>> _tasks;
	TWeakPtr<FGsTask> _curTask;
	E _curState;

public:
	void pushTask(E key, TSharedPtr<FGsTask> task)
	{
		_tasks.Emplace(key, task);
	}

	void clearTask(bool instanceClear = false)
	{
		if (instanceClear == false)
		{
			for (auto pair : _tasks)
			{
				if (pair.Value)
				{
					pair.Value->_finished = true;
					pair.Value->onFinish();
				}
			}
		}

		_tasks.Reset();
		_curState = static_cast<E>(0);
	}

	void doTasks(float delta = 0.f) // 호출 안 해도 됨
	{
		if (_curTask.IsValid())
		{
			_curTask.Pin()->update(delta);
		}
	}

	void ChangeTask(E key)
	{
		if (_curState == key) return;

		if (auto iter = _tasks.Find(key))
		{
			if (bool isChange = _curTask.IsValid() ? _curTask.Pin()->cond() : true)
			{
				if (_curTask.IsValid())
					_curTask.Pin()->onFinish();

				_curTask = *iter;
				_curState = key;

				if (_curTask.IsValid())
					_curTask.Pin()->onStart();
			}
		}
	}
};