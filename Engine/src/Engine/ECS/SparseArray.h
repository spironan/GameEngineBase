#pragma once
#include "pch.h"
#include "Engine/Memory/MemoryCommon.h"
#include "ECS_Utility.h"

namespace engine
{
    template<typename T, std::size_t MAX_SIZE, typename DenseAllocator = std::allocator<T>, 
        typename SparseAllocator = std::allocator<std::size_t>>
    class SparseContainer
    {


    private:
        /*using alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
        using alloc_traits = std::allocator_traits<alloc_type>;
        using alloc_pointer = typename alloc_traits::pointer;
        using alloc_const_pointer = typename alloc_traits::const_pointer;

        using bucket_alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<alloc_pointer>;
        using bucket_alloc_traits = std::allocator_traits<bucket_alloc_type>;
        using bucket_alloc_pointer = typename bucket_alloc_traits::pointer;*/

    public:
        using value_type = T;
        using dense_container = std::vector<value_type, DenseAllocator>;
        using size_type = std::size_t;
        using sparse_container = std::vector<size_type, SparseAllocator>;
        using index_container = std::array<size_type, MAX_SIZE>;
        using iterator = typename dense_container::iterator;

        static constexpr size_type MAX_ID = std::numeric_limits<size_type>::max();
    private:
        // the container of densely packed elements
        dense_container m_dense;
        // the container of index the densely packed elements are related to
        sparse_container m_sparse;
        // the container of index to the elements
        index_container m_index;

        /*alloc_type allocator;
        bucket_alloc_type bucket_allocator;
        bucket_alloc_pointer sparse;
        alloc_pointer packed;
        std::size_t bucket;
        std::size_t count;
        std::size_t reserved;*/


    public:
        ///------------------------------------
        /// Constructors
        ///------------------------------------
        /// // creating a sparse set; space is not reserved, requires resize.
        SparseContainer()
        {
            m_dense.reserve(MAX_SIZE);
            m_sparse.reserve(MAX_SIZE);
            std::fill(m_index.begin(), m_index.end(), MAX_ID);
        }

        bool IsValid(size_type const index) const
        {
            return index < MAX_SIZE;
        }

        bool IsAvailable(size_type const index) const
        {
            //if unused index return true
            if (IsValid(index) && m_index[index] >= m_dense.size())
                return true;
            //if the dense element located by this index does not belong to
            //this index return true, else return false
            return  m_sparse[m_index[index]] != index;
        }

        virtual ~SparseContainer() = default;

        T* At(size_type const index)
        {
            if (IsAvailable(index) == true)
                return nullptr;
            return &(m_dense[m_index[index]]);
        }

        T& AtIndex(size_type const index)
        {
            return m_dense[index];
        }

        typename sparse_container::value_type AtIndexSparse(size_type const index)
        {
            return m_sparse[index];
        }

        T* operator[](size_type const index)
        {
            return At(index);
        }

        size_type Size() const
        {
            return static_cast<size_type>(m_dense.size());
        }

        dense_container& GetContainer()
        {
            return m_dense;
        }
        T* insert(size_type const index, T& element)
        {
            if (IsAvailable(index) == false)
                return nullptr;
            m_dense.emplace_back(element);
            m_sparse.emplace_back(index);
            m_index[index] = m_dense.size() - 1;

            return At(index);
        }

        template <typename... args>
        T* emplace(size_type const index, args&&... arguementList)
        {
            if (IsAvailable(index) == false)
                return nullptr;
            m_dense.emplace_back(std::forward<args>(arguementList)...);
            m_sparse.emplace_back(index);
            m_index[index] = m_dense.size() - 1;

            return At(index);
        }

        virtual bool Remove(size_type const index)
        {
            if (IsAvailable(index))
                return false;

            size_type index_to_remove = m_index[index];
            size_type last_index = m_sparse.back();

            m_dense[index_to_remove] = m_dense.back();
            m_sparse[index_to_remove] = m_sparse.back();

            m_index[last_index] = index_to_remove;


            m_dense.pop_back();
            m_sparse.pop_back();
            return true;
        }

        void Clear()
        {
            m_dense.clear();
            m_sparse.clear();
            std::fill(m_index.begin(), m_index.end(), MAX_ID);
        }

        iterator begin() { return m_dense.begin(); }
        iterator end() { return m_dense.end(); }

        void Swap(size_type index1, size_type index2)
        {
            if (index1 >= m_dense.size() || index2 >= m_dense.size() || index1 == index2)
                return;
            std::swap(*(m_dense.begin() + index1), *(m_dense.begin() + index2));
            std::swap(*(m_sparse.begin() + index1), *(m_sparse.begin() + index2));
            std::swap(m_index[*(m_sparse.begin() + index1)], m_index[*(m_sparse.begin() + index2)]);
        }

        dense_container& GetDenseContainer()
        {
            return m_dense;
        }

        size_type GetIndex(Entity e)
        {
            if (IsAvailable((size_type)e))
                return Size();
            return m_index[e];
        }
    private:

    };

}