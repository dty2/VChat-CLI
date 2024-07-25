/*
 * Copyright (c) 2024 Hunter 执着
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "telescope.h"
#include "vchat.h"

int Chats::selected;
int Friends::selected;
int Inform::selected;

Chats::Chats(Telescope* telescope) {
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    clist->Add(Renderer([=](bool focused){ return filler(); }));
    for(auto& v : Info::info->friendinfo) {
      clist->Add(Renderer([=](bool focused){
        auto show = hbox(
          text("  " + v.second.friendname + " >"),
          filler(),
          text(Info::info->messageinfo[v.first].back().msg)
        );
        if(focused) return hbox(text("> "), show) | bgcolor(Color::Blue);
        else return show;
      }));
      for(auto vv : Info::info->messageinfo[v.first]) {
        Element show;
        if(vv.sender == v.first) {
          show = hbox(text(" > "), text(vv.msg), filler());
        } else if(vv.receiver == v.first){
          show = hbox(filler(), text(vv.msg), text(" < "));
        }
        previews[v.first] = window(text(" 󰐩  Preview "), vbox(show) | center, LIGHT);
      }
    }
    if(clist->ChildCount() == 1) {
      clist->DetachAllChildren();
      clist->Add(Renderer(
        [=](bool focused){ return text("  no message infomation...") | center; })
      );
    }
    return window(text(" 󰨝  Messages ") | center, clist->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14);
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if (event == Event::CtrlN) {
      if(selected != Info::info->messageinfo.size() - 1) selected ++;
    } else if (event == Event::CtrlP) {
      if(selected) selected --;
    } else if(event == Event::Return) { return true; } // TODO:
    return false;
  });
  this->list = elist;
}

Friends::Friends(Telescope* telescope) {
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    clist->Add(Renderer([=](bool focused){ return filler(); }));
    for(auto v : Info::info->friendinfo) {
      clist->Add(Renderer([=](bool focused){
        auto show = hbox(
          text("  " + v.second.friendname + ' '),
          filler()
        );
        if(focused) return hbox(text("> "), show) | bgcolor(Color::Blue);
        else return show;
      }));
      previews[v.first] = window(text(" 󰐩  Preview "), vbox(
        text("[     ID    ] :" + std::to_string(v.first)),
        text("[  Username ] :" + v.second.friendname)
      ) | center, LIGHT);
    }
    if(clist->ChildCount() == 1) {
      clist->DetachAllChildren();
      clist->Add(Renderer([=](bool focused){ return text("  no friends...") | center; }));
    }
    return window(text(" 󰨝  Friends ") | center, clist->Render(), LIGHT)
    | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 14);
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if(event == Event::CtrlP) {
      if (selected) selected --;
    } else if(event == Event::CtrlN) {
      if (selected != Info::info->friendinfo.size() - 1) selected ++;
    } else if(event == Event::Return) { return true; } // TODO:
    return false;
  });
  this->list = elist;
}

Inform::Inform(Telescope *telescope) {
  auto clist = Container::Vertical({}, &selected);
  auto rlist = Renderer(clist, [=]{
    clist->DetachAllChildren();
    for(auto& v : Info::info->requestaddlist) {
      previews[v.first] = window(text(" 󰐩  Preview "), vbox(
        text("[     ID    ] :" + std::to_string(v.first)),
        text("[  Username ] :" + v.second.first["username"].asString())
      ) | center, LIGHT);
      auto rbuts = Renderer([=](bool focused){
        Element show;
        if (focused)
          show = vbox(
            text(">username "
              + v.second.first["username"].asString()
              + "requests to add a friend "
            )
          ) | bgcolor(Color::Blue);
        else
          show = vbox(
            text("username "
              + v.second.first["username"].asString()
              + "requests to add a friend ")
          );
        return show;
      });
      clist->Add(rbuts);
      id[selected] = v.first;
    }
    if(!clist->ChildCount())
      clist->Add(Renderer([=](bool focused){ return text("📢 no inform..."); }));
    return clist->Render();
  });
  auto elist = CatchEvent(rlist, [&](Event event){
    if (event == Event::CtrlN) {
      if(selected != Info::info->requestaddlist.size() - 1) selected ++;
    } else if(event == Event::CtrlP) {
      if(selected) selected --;
    } else if(event == Event::Return) {
      telescope->vchat->createdialog(
        Container::Vertical({
          Button(" Yes ", [=]{ function->responseadd(id[selected], 1); }),
          Button(" No ", [=]{ function->responseadd(id[selected], 0); })
        })
      );
      return true;
    }
    return false;
  });
  this->list = elist;
}

Telescope::Telescope(Vchat *vchat_, int *toggle_)
  : chats(this), friends(this), toggle(toggle_) {
  Telescope::vchat = vchat_;
  auto cinput = Input(&ss, "");
  auto rinput = Renderer(cinput, [=]{
    return window(text("  搜索 ") | center, cinput->Render(), LIGHT);
  });
  input = rinput;
  auto clist = Container::Tab({chats.list, friends.list}, &selected);
  auto elist = CatchEvent(clist, [&](Event event){
    if(event == Event::CtrlJ) {
      if(selected == CHAT) selected ++;
    } else if(event == Event::CtrlK) {
      if(selected == FRIEND) selected --;
    }
    return false;
  });
  auto cmain = Container::Vertical({ elist, input }, &selected);
  auto rmain = Renderer(cmain, [=]{
    if (selected == CHAT)
      return hbox(cmain->Render(), chats.previews[Chats::selected]);
    else if (selected == FRIEND)
      return hbox(cmain->Render(), friends.previews[Friends::selected]);
    return text("Error");
  });
  auto emain = CatchEvent(rmain, [&](Event event){
    if (event == Event::CtrlN
        || event == Event::CtrlP
        || event == Event::CtrlJ
        || event == Event::CtrlK
        || event == Event::Return
      ) {
      clist->TakeFocus();
      return false;
    }
    else input->TakeFocus();
    return false;
  });
  this->content = emain | center;
}

Telescope::~Telescope() {}

Component Telescope::gettelescope(int selected_) {
  this->selected = selected_;
  return this->content;
}
