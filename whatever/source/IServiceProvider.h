#pragma once
#include <IService.h>
#include "util/RefPtr.h"

#include <concepts>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace wtv
{
	struct IServiceProvider : public IReferenceCounted
	{
		template<IServiceDerived Service>
		void AddService(const std::shared_ptr<Service>& service)
		{
			assert(m_services.find(service->GetServiceName()) == m_services.end());
			m_services.emplace(service->GetServiceName(), service);
		}

		template<IServiceDerived Service>
		Service* GetService()
		{
			auto it = m_services.find(Service::GetServiceNameStatic());
			assert(it != m_services.end());
			return static_cast<Service*>(it->second.get());
		}

		std::unordered_map<std::string, std::shared_ptr<IService>> m_services;
	};

}