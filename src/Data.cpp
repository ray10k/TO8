/* ========================================================================== */
/*                                                                            */
/*   Data.cpp                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "Data.h"

Data::Data(){
    wheelRadius = 215;
    weight = 70;
    
    clearAll();
}

void Data::addPulse(unsigned long long int pulseTime){
    if(prevTimeTmMode == 0 ) prevTimeTmMode = pulseTime;
    if(firstTime == 0) firstTime = pulseTime;
    if(prevTime == 0) prevTime = pulseTime;
    
    pulses++;       
    
    if(time != 0) {
        distance += wheelRadius;
        totalDistance += wheelRadius;
        speed = (wheelRadius * 1000000) / (pulseTime - prevTime);
        if(speed > maxSpeed) maxSpeed = speed;
    }
    
    time += (pulseTime - prevTimeTmMode);
    prevTimeTmMode = pulseTime;
    prevTime = pulseTime;
}

void Data::setPrevTime(unsigned long long int t) {
    prevTimeTmMode = t;
}

int Data::getSpeed(void){
    int tempTime = RTOS::run_time();
    if((tempTime - prevTime) > 1000000) {
        speed = (wheelRadius * 100000) / (tempTime - prevTime);
    }
    
    return speed * 36;
}

int Data::getAverage(void){   
    averageSpeed = distance / getDrivingTime();
    return averageSpeed * 36;
}

int Data::getMax(void){
    return maxSpeed * 36;
}

int Data::getTotalDistance(void){
    return totalDistance / 1000;//to KM
}

int Data::getDrivingTime(void){
    unsigned long long int t = RTOS::run_time();
    if(prevTimeTmMode == 0 ) prevTimeTmMode = t;
    time += (t - prevTimeTmMode);
    prevTimeTmMode = t;
    
    return time / 1000000; //to Min
}

int Data::getCalories(void){
    return ((getAverage()/100) / 68 * getDrivingTime() * weight) / 1000 ;
}

int Data::getDistance(void){
    return distance / 1000; //to KM
}

void Data::clearCurrent( void ) {
    prevTimeTmMode = prevTime = firstTime = 0;
    pulses = 0;
    
    speed = 0;
    maxSpeed = 0;
    averageSpeed = 0;
    distance = 0;
    time = 0;
}

void Data::clearAll( void ) {
    totalDistance = 0;
    
    clearCurrent();
}

bool Data::isError( void ) {
	if(getSpeed() >= 100000) return true;
	if(getDistance() >= 100000) return true;
	if(getDrivingTime() >= 360000 - 1) return true;
	if(getTotalDistance() >= 1000000) return true;
	if(getCalories() >= 10000) return true;
	
	return false;
}
