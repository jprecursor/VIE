//
//  LifeCell.cpp
//  VIE
//
//  Created by Jordan Hochenbaum on 3/16/13.
//
//

#include "LifeCell.h"


LifeCell::LifeCell(const String& name) : ImageButton(name)
{
    //do nothing, we just pass on to the base classes constructor in the initialization list
}

bool LifeCell::hitTest(int x, int y)
{
    bool allowsClicksOnComponent, allowsClicksOnChildComponents;                      //temp variables to store clickable state
    getInterceptsMouseClicks(allowsClicksOnComponent, allowsClicksOnChildComponents); //request clickable state and store in temp variables
    
    //if the component is not supposed to receive mouse clicks then return false
    //otherwise pass the hit test into the base classes hit test method
    if (!allowsClicksOnComponent || !allowsClicksOnChildComponents){
        return false;
    }else{
        return ImageButton::hitTest(x, y);
    }
}