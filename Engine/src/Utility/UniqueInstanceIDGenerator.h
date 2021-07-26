/************************************************************************************//*!
\file           UIID_Generator.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 07, 2021
\brief          Describes a generator that creates manages and generates unique IDs 
                that is reusable to allow for less rapid growth required in sparse Set.
                
                Generates an unique ID whenever called.
                The ID received cannot be perceived as what it is but instead split 
                into two parts : version and index. The individual results of 
                this can be retrieved by using the appropriate function that takes in the 
                ID to get its actual values.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <type_traits>
#include <vector>

namespace engine {
namespace utility
{
    /********************************************************************************//*!
     @brief    Describes a generator that creates manages and generates unique IDs 
               that is reusable to allow for less rapid growth required in sparse Set.
     
               Generates an unique ID whenever called.
               The ID received cannot be perceived as what it is but instead split 
               into two parts : version and index. The individual results of 
               this can be retrieved by using the appropriate function that takes in the 
               ID to get its actual values.
    *//*********************************************************************************/
    template<typename ID = std::size_t>
    class UIID_Generator final
    {
    public:

        //ensures that value_type is of an unsigned integral type
        using value_type = std::enable_if_t<std::is_unsigned_v<ID>, ID>;
        using const_value = value_type const;

        /****************************************************************************//*!
         @brief    Constructs the default constructor of UIID_Generator.
        *//*****************************************************************************/
        UIID_Generator() = default;

        /****************************************************************************//*!
         @brief    Destructs the default destructor of UIID_Generator.
        *//*****************************************************************************/
        ~UIID_Generator() = default;

        /****************************************************************************//*!
         @brief    Get the version of the ID.
         
         @param[in]    _ID
            The ID to get the version of a given generated ID.
         
         @return   The version of the ID.
        *//*****************************************************************************/
        static value_type GetVersion(const_value _ID);

        /****************************************************************************//*!
         @brief    Get the index of the ID.

         @param[in]    _ID 
            The ID to get the index of a given generated ID.

         @return   The index of the ID.
        *//*****************************************************************************/
        static value_type GetIndex(const_value _ID);

        /****************************************************************************//*!
         @brief    Check if a ID is valid.
         
         @param[in]    _ID
            The ID to check for validity.
         
         @return    True if the ID is valid, false otherwise.
        *//*****************************************************************************/
        bool IsValid(const_value _ID) const;

        /****************************************************************************//*!
         @brief    Generate a new ID.
         
         @return   A new ID.
        *//*****************************************************************************/
        value_type GenerateID();

        /****************************************************************************//*!
         @brief    Discards the ID if it is valid.
         
         @param[in]    _ID 
            The ID to discard.
        *//*****************************************************************************/
        void DiscardID(const_value _ID);

    private:
        
        /****************************************************************************//*!
         @brief    Creates a value_type-bits ID based on the provided version and index.
         
         @param[in]    _version 
            The number number of the ID.
         @param[in]    _index 
            The index of the ID.
         
         @return   The created ID.
        *//*****************************************************************************/
        static value_type createID(const_value _version, const_value _index);

        /****************************************************************************//*!
         @brief    Internal function to check if a ID is valid given its index and ID. 
         
         @param[in]    _index 
            The index of the ID.
         @param[in]    _ID 
            The ID to check for validity.
         
         @return   True if the ID is valid, false otherwise.
        *//*****************************************************************************/
        bool isValidID(const_value _index, const_value _ID) const;

        // the number of bytes that specified unsigned integral type is.
        static constexpr size_t typeBytes           = sizeof(value_type);

        // number of bytes of the whole object type
        static constexpr size_t idBytes             = typeBytes;
        // number of bytes to hold the index (maximum number of combinations)
        static constexpr size_t indexBytes          = typeBytes/2;
        // number of bytes to hold the version (maximum number of versions)
        static constexpr size_t versionBytes        = idBytes - indexBytes;


        static constexpr size_t bytesToBits         = 8;
        
        //calculate the bits of each type
        static constexpr size_t idBits              = idBytes       * bytesToBits;
        static constexpr size_t indexBits           = indexBytes    * bytesToBits;
        static constexpr size_t versionBits         = versionBytes  * bytesToBits;

        // bitmask used to help get the values of index and verseion
        static constexpr value_type indexBitmask    = (value_type{1} << indexBits) - 1;
        static constexpr value_type versionBitmask  = ~indexBitmask;

        // Array of all handles.
        std::vector<value_type> uniqueIDArray;
        // The number of handles that can be recycled.
        size_t recycleCounter = 0;
        // The index of the next ID to recycle.
        value_type nextIndex = 0;
    };

    template<typename ID>
    static typename UIID_Generator<ID>::value_type UIID_Generator<ID>::GetVersion(UIID_Generator<ID>::const_value _ID)
    {
        return _ID >> indexBits;
    }

    template<typename ID>
    static typename UIID_Generator<ID>::value_type UIID_Generator<ID>::GetIndex(UIID_Generator<ID>::const_value _ID)
    {
        return _ID & indexBitmask;
    }

    template<typename ID>
    bool UIID_Generator<ID>::IsValid(UIID_Generator<ID>::const_value _ID) const
    {
        value_type index = GetIndex(_ID);
        return isValidID(index, _ID);
    }

    // index
    // 0 1 2 3 4 5 6 7 8 9 
    // 1 4 5 8 0
    // 0 8 5 4 1

    template<typename ID>
    typename UIID_Generator<ID>::value_type UIID_Generator<ID>::GenerateID()
    {
        // If there are no indices that can be recycled
        // create a new handle and append it to uniqueIDArray.
        if (recycleCounter == 0)
        {
            // create a new ID therefore version will be 0 and value 
            // will be the size of the array
            value_type newID = createID(0, (value_type)uniqueIDArray.size());
            uniqueIDArray.push_back(newID);
            return newID;
        }

        // We need the index and version of the ID we are recycling.
        value_type index = nextIndex;                           //0
        value_type version = GetVersion(uniqueIDArray[index]);  //0

        // nextIndex will point to the index that the discarded ID 
        // was previously pointing to.
        nextIndex = GetIndex(uniqueIDArray[nextIndex]);         //0
        // Decrease recycleCounter.
        --recycleCounter;

        // Increase the version number of the recycled handle and 
        // store the new handle into uniqueIDArray.
        uniqueIDArray[index] = createID(version + 1, index);
        return uniqueIDArray[index];
    }

    template<typename ID>
    void UIID_Generator<ID>::DiscardID(UIID_Generator<ID>::const_value _ID)
    {
        // Find out the index and version of the handle we are discard.
        value_type index = GetIndex(_ID);           //0
        value_type version = GetVersion(_ID);       //0

        if (isValidID(index, _ID))
        {
            //first array element = 0000 0000
            //nextIndex -> 0
            

            // Use the index portion of this discarded ID to point to the index of the previous discarded ID.
            uniqueIDArray[index] = createID(version, nextIndex);
            // nextIndex now points the index of this discarded ID.
            nextIndex = index;
            // Increase the recycle counter.
            ++recycleCounter;
        }
    }
    

    // 0 0 0 0 | 0 0 0 0  
    // version    index
    // ========ID========

    template<typename ID>
    static typename UIID_Generator<ID>::value_type UIID_Generator<ID>::createID( UIID_Generator<ID>::const_value _version, UIID_Generator<ID>::const_value _index)
    {
        return (_version << indexBits) | (_index & indexBitmask);
    }

    template<typename ID>
    bool UIID_Generator<ID>::isValidID(  UIID_Generator<ID>::const_value _index, UIID_Generator<ID>::const_value _ID) const
    {
        return _index < uniqueIDArray.size() && uniqueIDArray[_index] == _ID;
    }


}} // namespace Phi::Utility