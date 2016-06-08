#include "Singleton.h"

namespace commonOptions {

auto Singleton::getInstance() -> Singleton& {
	static Singleton instance;
	return instance;
}


auto Singleton::register_on_before_save(std::function<void()> _callback) -> CallbackHandler {
	std::unique_lock<std::mutex> lock(mMutex);

	id += 1;
	mCallbacksOnBeforeSave.emplace_back(std::make_pair(id, _callback));
	return CallbackHandler { new CallbackOnExitHandler ([=] {
		std::unique_lock<std::mutex> lock(mMutex);
		for (auto iter = mCallbacksOnBeforeSave.begin(); iter != mCallbacksOnBeforeSave.end(); ++iter) {
			if (iter->first == id) {
				mCallbacksOnBeforeSave.erase(iter);
				return;
			}
		}
	}) };
}

auto Singleton::register_on_save(std::function<void()> _callback) -> CallbackHandler {
	std::unique_lock<std::mutex> lock(mMutex);

	id += 1;
	mCallbacksOnSave.emplace_back(std::make_pair(id, _callback));
	return CallbackHandler { new CallbackOnExitHandler ([=] {
		std::unique_lock<std::mutex> lock(mMutex);
		for (auto iter = mCallbacksOnSave.begin(); iter != mCallbacksOnSave.end(); ++iter) {
			if (iter->first == id) {
				mCallbacksOnSave.erase(iter);
				return;
			}
		}
	}) };
}

void Singleton::signal_save() {
	std::unique_lock<std::mutex> lock(mMutex);
	for (auto const& e: mCallbacksOnBeforeSave) {
		e.second();
	}

	for (auto const& e: mCallbacksOnSave) {
		e.second();
	}

}





}



