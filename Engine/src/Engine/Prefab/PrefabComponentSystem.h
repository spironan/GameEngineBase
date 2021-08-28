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
	 Check if the prefab is already made else add it into the prefab pool
	\param    filepath
	 use to generate data for the map
	*//**********************************************************************************/
		Entity AddPrefab(const std::string& filepath);
	/*********************************************************************************//*!
	\brief    
	 Returns a prefab when accessed using a filepath	 
	\param    filepath
	\return   
	 prefab head id
	*//**********************************************************************************/
		Entity GetPrefab(const std::string& filepath);
	/*********************************************************************************//*!
	\brief    
	 Creates the prefab using gameobject provided
	 head will be the start of the prefab (*not its children)
	 
	\param    filepath
	 prefab filepath
	\param    head
	 start of the new prefab object
	*//**********************************************************************************/
		void InstantiateFromPrefab(const std::string& filepath, GameObject& head);
	/*********************************************************************************//*!
	\brief    
	 The full creation flow of prefab making

	\param    filepath
	 target location of the prefab file
	\param    head
	 start of seralization
	*//**********************************************************************************/
		void MakePrefab(const std::string& filepath, GameObject& head);

	/*********************************************************************************//*!
	\brief    
	 Saves the prefab into file
	\param    go
	 Can be anypart of the prefab as long its connected
	 it will look for the head node then do a comparison with the main list
	 can be quite costly so do not use this function too much
	*//**********************************************************************************/
		void SavePrefab(GameObject& go);
	/*********************************************************************************//*!
	\brief
	 Saves all the prefab currently tracked
	*//**********************************************************************************/
		void SavePrefab();

	private:
		struct FileDetails
		{
			Entity head = 0;
			std::string filename;
		};
		std::map<utility::StringHash::size_type , FileDetails> m_prefabDetails;
	};

}
