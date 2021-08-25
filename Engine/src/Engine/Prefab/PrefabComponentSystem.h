#include "Engine/ECS/System.h"
#include <map>
#include "Utility/Hash.h"
namespace engine
{
	class PrefabComponent;
	class PrefabComponentSystem final :public System
	{
	public:
	/*-----------------------------------------------------------------------------*/
	/* Class Specific Type Aliases                                                 */
	/*-----------------------------------------------------------------------------*/
		using value_type = PrefabComponent;
		using value_reference = value_type&;
		using value_const_reference = value_type const&;

		using container_type = std::vector<value_type>;
		using container_reference = container_type&;
		using container_iterator = container_type::iterator;
		using container_const_iterator = container_type::const_iterator;

	/*-----------------------------------------------------------------------------*/
	/* Explicit Constructor                                                        */
	/*-----------------------------------------------------------------------------*/
		explicit PrefabComponentSystem(engine::ECS_Manager& _ECS_Manager) : System{ _ECS_Manager } {};

	/*********************************************************************************//*!
	\brief
	 Saves all the prefab currently tracked
	*//**********************************************************************************/
		void SavePrefab();
	/*********************************************************************************//*!
	\brief    
	 Check if the prefab is already made else add it into the prefab pool
	\param    filepath
	 use to generate data for the map
	*//**********************************************************************************/
		void AddPrefab(const std::string& filepath);
	/*********************************************************************************//*!
	\brief    
	 Returns a prefab when accessed using a filepath	 
	\param    filepath
	\return   
	 prefab head id
	*//**********************************************************************************/
		Entity GetPrefab(const std::string& filepath);
	private:
		struct FileDetails
		{
			Entity head = 0;
			std::string filename;
		};
		std::map<utility::StringHash::size_type , FileDetails> m_prefabDetails;
	};

}
