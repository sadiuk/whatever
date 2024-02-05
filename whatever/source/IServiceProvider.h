#pragma once
#include <IService.h>

#include <concepts>
#include <unordered_map>
#include <memory>


struct IServiceProvider
{
	template<IServiceDerived Service>
	void AddService(const std::shared_ptr<Service>& service)
	{
		assert(m_services.find(m_services.begin(), m_services.end(), service) == m_services.end());
		m_services.emplace(service->GetServiceName(), service);
	}

	template<IServiceDerived Service>
	Service* GetService()
	{
		auto it = m_services.find(Service::GetServiceNameStatic());
		assert(it != m_services.end());
		return static_cast<Service*>(it->second.get());
	}

	std::map<std::string, std::shared_ptr<IService>> m_services;
};

