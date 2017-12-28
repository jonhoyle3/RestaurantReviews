//
//  JHTSRestaurant.cpp
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include "JHTSRestaurant.h"
#include "JHTSReview.h"


// Private Constructor
JHTSRestaurant::JHTSRestaurant(const string &name, int zipCode, JHTSGUID byUser)
{
    mName = name;
    mZipCode = zipCode;
    mByUser = byUser;
    mCachedListOfReviews = new set<JHTSGUID>;
}

// Virtual Destructor
JHTSRestaurant::~JHTSRestaurant()
{
}

// Get a Restaurant from the database (possibly creating one)
JHTSRestaurant *JHTSRestaurant::getRestaurant(const string &name, int zipCode)
{
    JHTSRestaurant *restaurant;

    // Look for it in our list
    for (auto ptr = mDatabaseObjectList.begin(); ptr != mDatabaseObjectList.end(); ptr++)
    {
        restaurant = dynamic_cast<JHTSRestaurant *>(*ptr);
        
        if ((restaurant != NULL) && (restaurant->mName == name) && (restaurant->mZipCode == zipCode))
            return restaurant;
    }
    
    return NULL;
}

int JHTSRestaurant::averageStarRating() const
{
    double sumRating = 0.0;
    set<JHTSGUID> reviewList = listOfReviews();
    JHTSReview *review;
    
    if (reviewList.size() > 0)
    {
        for (auto ptr = reviewList.begin(); ptr != reviewList.end(); ptr++)
        {
            review = JHTSReview::getReview(*ptr);
            sumRating += review->getStarRating();
        }
        
        sumRating = (sumRating / reviewList.size()) + 0.5;
    }
    
    return (int) sumRating;
}

set<JHTSGUID> JHTSRestaurant::listOfReviews() const
{
    // If we have an invalid object, recalculate the list
    if (!isValid())
    {
        delete mCachedListOfReviews;
        (const_cast<JHTSRestaurant *>(this))->mCachedListOfReviews = NULL;
    }
    
    // If we have to recalculate the cache
    if (mCachedListOfReviews == NULL)
    {
        set<JHTSGUID> *reviewList = new set<JHTSGUID>;
        JHTSReview *review;

        for (auto ptr = mDatabaseObjectList.begin(); ptr != mDatabaseObjectList.end(); ptr++)
        {
            review = dynamic_cast<JHTSReview *>(*ptr);
            
            if ((review != NULL) && (review->getRestaurantGUID() == mGUID))
                reviewList->insert(review->guid());
        }
        
        (const_cast<JHTSRestaurant *>(this))->mCachedListOfReviews = reviewList;
    }
    
    return *mCachedListOfReviews;
}

set<JHTSGUID> JHTSRestaurant::listOfReviewsWithRatingOrHigher(int starRating) const
{
    set<JHTSGUID> reviewList = listOfReviews();
    JHTSReview *review;
    
    for (auto ptr = reviewList.begin(); ptr != reviewList.end(); ptr++)
    {
        review = JHTSReview::getReview(*ptr);
        
        if ((review != NULL) && (review->getStarRating() >= starRating))
            reviewList.insert(review->guid());
    }
    
    return reviewList;
}

set<JHTSGUID> JHTSRestaurant::listOfRestaurantsByName(const string &name, int zipCode)
{
    set<JHTSGUID> restaurantList = listOfRestaurants();
    set<JHTSGUID> restaurantNameList;
    JHTSRestaurant *restaurant;
    
    for (auto ptr = restaurantList.begin(); ptr != restaurantList.end(); ptr++)
    {
        restaurant = getRestaurant(*ptr);

        if ((restaurant != NULL) && (restaurant->mName == name))
            if ((zipCode == 0) || (restaurant->mZipCode == zipCode))
                restaurantList.insert(restaurant->mGUID);
    }
    
    return restaurantList;
}

set<JHTSGUID> JHTSRestaurant::listOfRestaurantsByZipCode(int zipCode, int mileRange, int minStarRating)
{
    set<JHTSGUID> restaurantList = listOfRestaurants();
    set<JHTSGUID> restaurantZipList;
    JHTSRestaurant *restaurant;
    
    for (auto ptr = restaurantList.begin(); ptr != restaurantList.end(); ptr++)
    {
        restaurant = getRestaurant(*ptr);

        if ((restaurant != NULL) && (ZipCodeDistance(restaurant->mZipCode, zipCode) <= mileRange))
            if ((minStarRating == -1) || (restaurant->averageStarRating() >= minStarRating))
                restaurantZipList.insert(restaurant->mGUID);
    }
    
    return restaurantZipList;
}

bool JHTSRestaurant::isValid() const
{
    if (!JHTSDatabaseObject::isValid())
        return false;
        
    if (mCachedListOfReviews != NULL)
    {
        set<JHTSGUID> reviewList = *mCachedListOfReviews;
        JHTSReview *review;

        for (auto ptr = reviewList.begin(); ptr != reviewList.end(); ptr++)
        {
            review = JHTSReview::getReview(*ptr);
            if (review == NULL) return false;
            if (review->getRestaurantGUID() != mGUID)
                return false;
        }
    }

    return true;
}

void JHTSRestaurant::print(ostream &stream) const
{
    set<JHTSGUID> reviewList = listOfReviews();

    stream << "Name: \"" << mName << "\"" << endl;
    stream << "Zip: " << mZipCode << endl;
    stream << "By: " << mByUser << endl;
    stream << "Reviews: ";

    for (auto ptr = reviewList.begin(); ptr != reviewList.end(); ptr++)
        stream << (int) *ptr << ", ";
    
    stream << endl;
    stream << "Validity: " << (isValid() ? "true" : "false") << endl;
}
