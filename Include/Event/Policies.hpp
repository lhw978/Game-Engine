/*
    Copyright (c) 2018-2020 Piotr Doan. All rights reserved.
*/

#pragma once

/*
    Event Policies
*/

namespace Event
{
    enum class SubscriptionPolicy
    {
        // Keep existing subscription and fail subsequent subscription attempts.
        RetainSubscription,

        // Remove existing subscription to guarantee subsequent subscription attempts.
        ReplaceSubscription,
    };

    enum class PriorityPolicy
    {
        // Give normal priority by subscribing at the end of receiver list.
        InsertBack,

        // Give highest priority by subscribing at the beginning of receiver list.
        InsertFront,
    };
}
