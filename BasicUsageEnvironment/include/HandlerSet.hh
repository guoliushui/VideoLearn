/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 1996-2018 Live Networks, Inc.  All rights reserved.
// Basic Usage Environment: for a simple, non-scripted, console application
// C++ header

#ifndef _HANDLER_SET_HH
#define _HANDLER_SET_HH

#ifndef _BOOLEAN_HH
#include "Boolean.hh"
#endif

////////// HandlerSet (etc.) definition //////////

class HandlerDescriptor {
  HandlerDescriptor(HandlerDescriptor* nextHandler);
  virtual ~HandlerDescriptor();

public:
  int socketNum;
  int conditionSet;
  TaskScheduler::BackgroundHandlerProc* handlerProc;
  void* clientData;

private:
  // Descriptors are linked together in a doubly-linked list:(gxq HandlerDescriptor以双向链表的形式被连接起来)
  friend class HandlerSet;
  friend class HandlerIterator;
  HandlerDescriptor* fNextHandler;
  HandlerDescriptor* fPrevHandler;
};

class HandlerSet {
public:
  HandlerSet();
  virtual ~HandlerSet();

  //gxq 把一个新的sock插入双向链表中，在HandlerDescriptor对象中建立socket和handler之间的关系；
  void assignHandler(int socketNum, int conditionSet, TaskScheduler::BackgroundHandlerProc* handlerProc, void* clientData);

  //gxq 删除 相应的链表节点
  void clearHandler(int socketNum);

  //gxq  修改套接字值
  void moveHandler(int oldSocketNum, int newSocketNum);

private:

	//gxq 根据套接字查找相应的处理函数，没有找到则返回NULL
  HandlerDescriptor* lookupHandler(int socketNum);

private:
  friend class HandlerIterator;

  //gxq 双向链表的头指针 
  HandlerDescriptor fHandlers;
};

class HandlerIterator {
public:
  HandlerIterator(HandlerSet& handlerSet);
  virtual ~HandlerIterator();

  HandlerDescriptor* next(); // returns NULL if none
  void reset();

private:
  HandlerSet& fOurSet;
  HandlerDescriptor* fNextPtr;
};

#endif
