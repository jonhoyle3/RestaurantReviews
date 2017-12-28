//
//  JHTSTest.cpp
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include "JHTSRestaurant.h"
#include "JHTSReview.h"
#include "JHTSUser.h"

void JHTSTest()
{
    printObjects();
    JHTSUser *jon = new JHTSUser("Hoyle", "Jon", 15228);
    printObjects();
    JHTSUser *steve = new JHTSUser("Jobs", "Steve", 15228);
    printObjects();
    JHTSGUID mcD = jon->addRestaurant("MacDonalds", 15201);
    printObjects();
    JHTSGUID wen = jon->addRestaurant("Wendys", 15201);
    printObjects();
    JHTSGUID mcDReview = jon->addReview(3, "It was okay.", mcD);
    printObjects();
    JHTSGUID mcDReview2 = jon->addReview(4, "It was better.", mcD);
    printObjects();
    JHTSGUID bkReview = steve->addReview(2, "Eh.", "Burger King", 15102);
    printObjects();
    JHTSGUID mcDReview3 = steve->addReview(1, "No", mcD);
    printObjects();
    steve->deleteReview(mcDReview3);
    printObjects();
}

