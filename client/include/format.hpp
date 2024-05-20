// Copyright 2024 Arthur Hunter. All rights reserved.
// Use of this source code is governed by the GPL-3.0 license 
// that can be found in the LICENSE file.

# ifndef FORMAT_HPP 
# define FORMAT_HPP 

#include "common.hpp"

// message info
typedef struct Message
{
  std::string message;
  // other info ...
}Message;

// user Information
typedef struct UserInfo
{
  std::string username;
  // other info ...
}UserInfo;

// current account Information
typedef struct Account
{
  UserInfo persionalinfo;
  std::list<UserInfo> friends;
  std::unordered_map<std::string, Message> messages;
  // other info ...
}Account;

# endif
