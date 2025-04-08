#pragma once

// Macro to disable copy and move
#define DISABLE_COPY_MOVE(Class)              \
    Class(const Class&) = delete;              \
    Class& operator=(const Class&) = delete;   \
    Class(Class&&) = delete;                   \
    Class& operator=(Class&&) = delete;
