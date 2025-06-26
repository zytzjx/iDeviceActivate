#pragma once
#include <map>
#include <string>
#include <mutex>
#include <windows.h>

class CDeviceList {
public:
    // 添加设备
    void add_device(const std::string& key, HANDLE device) {
        std::lock_guard<std::mutex> lock(mutex_);
        devices_[key] = device;
    }

    // 移除设备
    void remove_device(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        devices_.erase(key);
    }
	bool check_device(const std::string& key) const {
		std::lock_guard<std::mutex> lock(mutex_);
		return devices_.find(key) != devices_.end();
	}

	std::string getAnyOne() const {
		std::lock_guard<std::mutex> lock(mutex_);
		if (!devices_.empty()) {
			return devices_.begin()->first; // 返回第一个设备的key
		}
		return ""; // 如果没有设备，返回空字符串
	}
    // 查找设备
    bool find_device(const std::string& key, HANDLE& device) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = devices_.find(key);
        if (it != devices_.end()) {
            device = it->second;
            return true;
        }
        return false;
    }

    // 获取设备数量
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return devices_.size();
    }

    // 清空设备列表
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        devices_.clear();
    }

    // 遍历所有设备（只读）
    template<typename Func>
    void for_each(Func func) const {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& pair : devices_) {
            func(pair.first, pair.second);
        }
    }

private:
    mutable std::mutex mutex_;
    std::map<std::string, HANDLE> devices_;
};