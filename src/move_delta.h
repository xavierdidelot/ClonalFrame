/***************************************************************************
 *   Copyright (C) 2011 by Xavier Didelot   *
 *   xavier.didelot@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef WBMOVE_DELTA_H
#define WBMOVE_DELTA_H

#include "move.h"
#include <gsl/gsl_sf_exp.h>

namespace wb {

    /**
@brief Updates the value of delta
@author Xavier Didelot
*/
    class Move_delta : public Move
    {
    public:
        Move_delta(gsl_rng * rng,Param * p);
        ~Move_delta();
        void move(Param *p);///<Execute the move on the given parameters
        //double move2(Param *p);

    };

}
#endif
