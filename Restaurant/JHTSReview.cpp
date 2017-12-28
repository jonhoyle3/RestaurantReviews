//
//  JHTSReview.cpp
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include "JHTSReview.h"
#include "JHTSRestaurant.h"
#include "JHTSUser.h"


// Constructor
JHTSReview::JHTSReview(int starRating, string reviewText, JHTSGUID userGUID, JHTSGUID restaurantGUID)
{
    mStarRating = starRating;
    mReviewText = reviewText;
    mUserGUID = userGUID;
    mRestaurantGUID = restaurantGUID;
    mMostRecentUpdate = time(NULL);
}

// Virtual Destructor
JHTSReview::~JHTSReview()
{
}

// Reconstructor
void JHTSReview::operator()(int starRating, string reviewText)
{
    mStarRating = starRating;
    mReviewText = reviewText;
    mMostRecentUpdate = time(NULL);
}

void JHTSReview::setStarRating(int starRating)
{
    mStarRating = starRating;
    mMostRecentUpdate = time(NULL);
}

void JHTSReview::setReviewText(const string &reviewText)
{
    mReviewText = reviewText;
    mMostRecentUpdate = time(NULL);
}

// Get a Review from the database for user and restaurant
JHTSReview *JHTSReview::getReview(JHTSGUID userGUID, JHTSGUID restaurantGUID)
{
    // Look for it in our list
    for (auto ptr = mDatabaseObjectList.begin(); ptr != mDatabaseObjectList.end(); ptr++)
    {
        JHTSReview *review = dynamic_cast<JHTSReview *>(*ptr);
        
        if ((review != NULL) && (review->mUserGUID == userGUID) && (review->mRestaurantGUID == restaurantGUID))
            return review;
    }
    
    return NULL;
}

bool JHTSReview::isValid() const
{
    if (!JHTSDatabaseObject::isValid())
        return false;
    
    JHTSUser *user = JHTSUser::getUser(mUserGUID);
    if (user == NULL) return false;
    set<JHTSGUID> userReviewList = user->listOfReviews();
    if (userReviewList.find(mGUID) == userReviewList.end())
        return false;
    
    JHTSRestaurant *restaurant = JHTSRestaurant::getRestaurant(mRestaurantGUID);
    if (restaurant == NULL) return false;
    set<JHTSGUID> restaurantReviewList = restaurant->listOfReviews();
    if (restaurantReviewList.find(mGUID) == restaurantReviewList.end())
        return false;

    return true;
}

void JHTSReview::print(ostream &stream) const
{
    stream << "Rating: " << mStarRating << endl;
    stream << "Text: \"" << mReviewText << "\"" << endl;
    
    JHTSUser *user = JHTSUser::getUser(mUserGUID);
    stream << "by: " << *user << endl;
    
    JHTSRestaurant *restaurant = JHTSRestaurant::getRestaurant(mRestaurantGUID);
    stream << "for: " << *restaurant << endl;
}
