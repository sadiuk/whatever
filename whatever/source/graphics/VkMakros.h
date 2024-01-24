#pragma once

#define ASSERT_VK_SUCCESS_ELSE_RET0(x) if((x) != VK_SUCCESS) { assert(false); return 0; }

#define ASSERT_VK_SUCCESS_ELSE_RET(x) if((x) != VK_SUCCESS) { assert(false); return; }

#define ASSERT_VK_SUCCESS(x) if((x) != VK_SUCCESS) { assert(false); }