#pragma once

#define ASSERT_VK_SUCCESS_ELSE_RET0(x) if((x) != VK_SUCCESS) { assert(false); return 0; }