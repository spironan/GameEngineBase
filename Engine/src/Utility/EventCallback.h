/************************************************************************************//*!
\file          EventCallback.cpp
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          Aug 31, 2021
\brief         Defines a template class that represents an a collection of function
               pointers that can be easily added to and manipulated. Inspired by C#'s
               event and delegate system.
 
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <vector>     // std::vector
#include <functional> // std::function

namespace engine
{
    /********************************************************************************//*!
     @brief    A class template that represents an a collection of function
               pointers that can be easily added to and manipulated, and have the
               function pointers called easily.

               Inspired by C#'s event and delegate system, it should be used as such.
     
     @tparam    Args
         Parameters that are needed for the EventCallback.
    *//*********************************************************************************/
    template<typename... Args>
    class EventCallback
    {
      public:
        /*-----------------------------------------------------------------------------*/
        /* Type Definitions                                                            */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief    Type of the function that this EventCallback will take in.
        *//*****************************************************************************/
        using DelegateFormat = void(Args ...);
        /****************************************************************************//*!
         @brief    Type of the std::function wrapper for functions of the type
                   DelegateFormat.
        *//*****************************************************************************/
        using Delegate = std::function<DelegateFormat>;
        
        /*-----------------------------------------------------------------------------*/
        /* Constructors                                                                */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief    Default Constructor
        *//*****************************************************************************/
        EventCallback()             = default;
        virtual ~EventCallback()    = default;

        //// Prevent Copying and Moving
        //EventCallback(EventCallback const&)             = delete;
        //EventCallback(EventCallback const&&)            = delete;
        //EventCallback& operator=(EventCallback const&)  = delete;
        
        /*-----------------------------------------------------------------------------*/
        /* Usage Functions                                                             */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief    Function to add a Delegate to this EventCallback.

                   If attempting to add a member function of a class, lambdas/anonymous
                   functions must be used to capture the object pointer.
                   E.g.: [ptr](){} or [this](){}
         
         @throw         std::invalid_argument
             Thrown if a nullptr was provided to add into the event.
         @param[in]    delegate
             The delegate to add to this EventCallback. Multiple of the same Delegate can be
             added to the same EventCallback with no repercussions.
        *//*****************************************************************************/
        void AddHandler(Delegate delegate);
        /****************************************************************************//*!
         @brief    Function to remove a Delegate from this EventCallback.
                   Unless specified, if multiple of the same Delagate exist in this EventCallback,
                   only one of them will be removed.

                   If it does not exist in the event, nothing will happen.
         
         @param[in]    delegate
             The Delegate to remove from this EventCallback.
         @param[in]    removeAll
             Whether or not to remove all of the same Delegates in this EventCallback.
        *//*****************************************************************************/
        void RemoveHandler(Delegate delegate, bool removeAll = false);        
        /****************************************************************************//*!
         @brief    Function that executes all added Delegates in the EventCallback.
         
         @param[in]    args
             Varadic argument list for the required parameters to call raise this event.
        *//*****************************************************************************/
        void Invoke(Args ... args) const;
        /****************************************************************************//*!
         @brief    Removes all added Delegates from this EventCallback.
        *//*****************************************************************************/
        void Clear();
        
        /*-----------------------------------------------------------------------------*/
        /* Overloaded Operators                                                        */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief    Overloaded += operator that acts as a shorthand of AddHandler().

                   If attempting to add a member function of a class, lambdas/anonymous
                   functions must be used to capture the object pointer.
                   E.g.: [ptr](){} or [this](){}
         
         @throw         std::invalid_argument
             Thrown if a nullptr was provided to add into the event.
         @param[in]    delegate
             The delegate to add to this EventCallback. Multiple of the same Delegate can be
             added to the same EventCallback with no repercussions.
        *//*****************************************************************************/
        EventCallback& operator+=(Delegate delegate);
        /****************************************************************************//*!
         @brief    Overloaded -= operator that acts as a shorthand of RemoveHandler().
         
         @param[in]    delegate
             The Delegate to remove from this EventCallback.
        *//*****************************************************************************/
        EventCallback& operator-=(Delegate delegate);
        /****************************************************************************//*!
         @brief    Overloaded () operator that acts as a shorthand of Invoke().
         
         @param[in]    args
             Varadic argument list for the required parameters to call raise this event.
        *//*****************************************************************************/
        const EventCallback& operator()(Args ... args) const;
        
      private:
        std::vector<Delegate> delegates;
    };

    template <typename ... Args>
    void EventCallback<Args...>::AddHandler(Delegate delegate)
    {
        //// Prevent invalid delegates
        //if (delegate == nullptr)
        //{
        //    throw std::invalid_argument("Attempted to add a null delegate!");
        //}
        // Add the delegate
        delegates.push_back(delegate);
    }

    template <typename ... Args>
    void EventCallback<Args...>::RemoveHandler(Delegate delegate, bool removeAll)
    {
        // Look for the delegate and remove
        for (typename std::vector<Delegate>::iterator it = delegates.begin(); it != delegates.end();)
        {
            if (it->target<DelegateFormat>() == delegate.target<DelegateFormat>())
            {
                // Remove and set the next iterator accordingly
                it = delegates.erase(it);

                // Removed one, that's it
                if (!removeAll)
                {
                    break;
                }
            }
            else
            {
                // No match, move on to the next
                ++it;
            }
        }
    }

    template <typename ... Args>
    void EventCallback<Args...>::Invoke(Args... args) const
    {
        // Execute all delegates
        for (auto delegate : delegates)
        {
            delegate(args ...);
        }
    }

    template <typename ... Args>
    void EventCallback<Args...>::Clear()
    {
        // Clear all delegates
        delegates.clear();
    }

    template <typename ... Args>
    EventCallback<Args...>& EventCallback<Args...>::operator+=(Delegate delegate)
    {
        AddHandler(delegate);
        return *this;
    }
    
    template <typename ... Args>
    EventCallback<Args...>& EventCallback<Args...>::operator-=(Delegate delegate)
    {
        RemoveHandler(delegate);
        return *this;
    }
    
    template <typename ... Args>
    const EventCallback<Args...>& EventCallback<Args...>::operator()(Args... args) const
    {
        Invoke(args ...);
        return *this;
    }

} // namespace engine
