//
//  LifeCell.h
//  VIE
//
//  Created by Jordan Hochenbaum on 3/16/13.
//
//

#ifndef __VIE__LifeCell__
#define __VIE__LifeCell__

#include "JuceHeader.h"

class LifeCell : public ImageButton
{
    
public:
    LifeCell(const String& name = String::empty);
    
private:
    
    bool hitTest(int x, int y); //overrideing hitTest for extra logic to avoid clicking when not listening to the mouse is set
        
};

#endif /* defined(__VIE__LifeCell__) */
