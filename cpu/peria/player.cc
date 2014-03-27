#include "player.h"

#include <algorithm>
#include <cstring>
#include <glog/logging.h>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "base.h"

void Player::CopyFrom(const Player& player) {
  field_.CopyFrom(player.field());
  sequence_ = player.sequence_;
  state_ = player.state();
  score_ = player.score();
  x_ = player.get_x();
  y_ = player.get_y();
  r_ = player.get_r();
  ojama_ = player.ojama();
}

void Player::SetColorSequence(const string& colors) {
  sequence_.resize(colors.size());
  for (size_t i = 0; i < colors.size(); ++i)
    sequence_[i] = colors[i] - '0';
}

double Player::GetBestControl(Control* control, string* message) {
  vector<Control> controls;
  GetControls(&controls);

  vector<Player> children;
  double value = 0;
  for (size_t i = 0; i < controls.size(); ++i) {
    string msg;
    Player child(*this);
    double val = child.ApplyControl(controls[i], &msg);
    if (val > value) {
      value = val;
      if (control)
        *control = controls[i];
      if (message)
        *message = msg;
    }
  }

  return value;
}

double Player::ApplyControl(const Control& control, string* message) {
  field_.Put(control.first, y_, control.second, sequence_.substr(0, 2));
  // Remove controlling puyos.
  sequence_ = sequence_.substr(2);
  int chains = 1;
  int score = 0;
  int frame = 0;
  field_.Simulate(&chains, &score, &frame);

  if (sequence_.size() > 1 && score == 0)
    return GetBestControl(NULL, NULL);
  return Evaluate(score, frame, message);
}

double Player::Evaluate(int score, int frame, string* message) {
  return static_cast<double>(score);
}

namespace {
void Insert(int x, int y, int r, set<Player::Position>* stack,
            set<Player::Position>* queue) {
  Player::Position pos(Player::Control(x, r), y);
  if (stack->find(pos) != stack->end())
    return;
  stack->insert(pos);
  queue->insert(pos);
}
}  // namespace

void Player::GetControls(vector<Control>* controls) {
  typedef set<Position>::iterator Iterator;
  set<Position> stack;
  set<Position> queue;

  queue.insert(Position(Control(x_, r_), y_));
  while (!queue.empty()) {
    Iterator itr = queue.begin();
    int x = itr->first.first;
    int r = itr->first.second;
    int y = itr->second;
    queue.erase(itr);

    switch (r) {
    case 0: {
      if (field_.IsEmpty(x + 1, y) && field_.IsEmpty(x + 1, y + 1))  // Right
        Insert(x + 1, y, 0, &stack, &queue);
      if (field_.IsEmpty(x - 1, y) && field_.IsEmpty(x - 1, y + 1))  // Left
        Insert(x - 1, y, 0, &stack, &queue);
      if (field_.IsEmpty(x + 1, y))  // Turn Right
        Insert(x, y, 1, &stack, &queue);
      else if (field_.IsEmpty(x - 1, y))
        Insert(x - 1, y, 1, &stack, &queue);
      else
        Insert(x, y + 1, 2, &stack, &queue);
      if (field_.IsEmpty(x - 1, y))  // Turn Left
        Insert(x, y, 3, &stack, &queue);
      else if (field_.IsEmpty(x + 1, y))
        Insert(x + 1, y, 3, &stack, &queue);
    }
    case 1: {
      if (field_.IsEmpty(x + 2, y))  // Right
        Insert(x + 1, y, 1, &stack, &queue);
      if (field_.IsEmpty(x - 1, y))  // Left
        Insert(x - 1, y, 1, &stack, &queue);
      if (field_.IsEmpty(x, y - 1))  // Turn Right
        Insert(x, y, 2, &stack, &queue);
      else if (field_.IsEmpty(x, y + 1) && y < Field::kHeight + 2)
        Insert(x - 1, y, 2, &stack, &queue);
      if (field_.IsEmpty(x, y + 1))  // Turn Left
        Insert(x, y, 0, &stack, &queue);
    }
    case 2: {
      if (field_.IsEmpty(x + 1, y) && field_.IsEmpty(x + 1, y - 1))  // Right
        Insert(x + 1, y, 2, &stack, &queue);
      if (field_.IsEmpty(x - 1, y) && field_.IsEmpty(x - 1, y - 1))  // Left
        Insert(x - 1, y, 2, &stack, &queue);
      if (field_.IsEmpty(x - 1, y))  // Turn Right
        Insert(x, y, 3, &stack, &queue);
      else if (field_.IsEmpty(x + 1, y))
        Insert(x + 1, y, 3, &stack, &queue);
      else
        Insert(x, y - 1, 0, &stack, &queue);
      if (field_.IsEmpty(x + 1, y))  // Turn Left
        Insert(x, y, 1, &stack, &queue);
      else if (field_.IsEmpty(x - 1, y))
        Insert(x - 1, y, 1, &stack, &queue);
    }
    case 3: {
      if (field_.IsEmpty(x + 1, y))  // Right
        Insert(x + 1, y, 3, &stack, &queue);
      if (field_.IsEmpty(x - 2, y))  // Left
        Insert(x - 1, y, 3, &stack, &queue);
      if (field_.IsEmpty(x, y + 1))  // Turn Right
        Insert(x, y, 0, &stack, &queue);
      if (field_.IsEmpty(x, y - 1))  // Turn Left
        Insert(x, y, 2, &stack, &queue);
      else if (field_.IsEmpty(x + 1, y) && y < Field::kHeight + 2)
        Insert(x, y + 1, 2, &stack, &queue);
    }
    }
  }

  controls->clear();
  for (Iterator itr = stack.begin(); itr != stack.end(); ++itr)
    controls->push_back(itr->first);
  sort(controls->begin(), controls->end());
  vector<Control>::iterator itr = unique(controls->begin(), controls->end());
  controls->erase(itr, controls->end());
}
