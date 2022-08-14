#pragma once

#include <xhash>

namespace Pistachio {

	class UUID {
	public:
		// UUID id; or UUID id = x;
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }

	private:
		// usually 128-bit, but 64 should be enough for this engine
		uint64_t m_UUID;

	};

}

// establish a hash for UUID so that it can be used for unordered_map
namespace std {

	template<>
	struct hash<Pistachio::UUID> {
		std::size_t operator()(const Pistachio::UUID& uuid) const {
			// redirect to uint64_t hash
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};


}