/*
 * Copyright 2020 Codethink Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BL_ACQ_TIMER_H
#define BL_ACQ_TIMER_H

#include <stdint.h>

#include "common/error.h"


typedef struct bl_acq_timer_s bl_acq_timer_t;

extern bl_acq_timer_t *bl_acq_tim1;

void bl_acq_timer_init(uint32_t bus_freq);

enum bl_error bl_acq_timer_configure(bl_acq_timer_t *timer, uint32_t frequency);

void bl_acq_timer_enable(bl_acq_timer_t *timer);
void bl_acq_timer_disable(bl_acq_timer_t *timer);

void bl_acq_timer_start(const bl_acq_timer_t *timer);
void bl_acq_timer_stop(const bl_acq_timer_t *timer);

void bl_acq_timer_start_all(void);
void bl_acq_timer_stop_all(void);

#endif

