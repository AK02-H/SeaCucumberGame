/************************************************/
/*	Sea Cucumber Game	    	                */
/*	Programmer : Niblet, Amin Khan		        */
/*	Date : 11th May, 2022	                    */
/*	Email : akhan2019@elam.co.uk                */
/* 	Webpages : https://niblet.itch.io/		    */
/*             https://github.com/Niblet-0		*/
/* 	Discord : Niblet#7617		                */
/* 	                    		                */
/* 	Made for Teesside university		        */
/* 	Uhh, don't redistribute source code         */
/* 	and other copyright stuff                   */
/* 	                                            */
/************************************************/

#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Collider	//interface
{
public:
	virtual vector<FloatRect> CollisionPoints() = 0;


	virtual bool DoesCollide(Collider*) = 0;	//Pure virtual
	//virtual bool DoesCollide(FloatRect); //Impure virtual
	//virtual bool DoesCollide(FloatRect, Collider*); //Impure virtual

};