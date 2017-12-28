//
//  JHTSUser.cpp
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include "JHTSUser.h"
#include "JHTSReview.h"

// Constructor
JHTSUser::JHTSUser(const char *lastName, const char *firstName, int zipCode)
{
    initUser(string(lastName), string(firstName), zipCode);
}

// Constructor
JHTSUser::JHTSUser(const string &lastName, const string &firstName, int zipCode)
{
    initUser(lastName, firstName, zipCode);
}

// Virtual Destructor
JHTSUser::~JHTSUser()
{
}

// Default Initializer
void JHTSUser::initUser(const string &lastName, const string &firstName, int zipCode)
{
    mLastName = lastName;
    mFirstName = firstName;
    mZipCode = zipCode;
    mCachedListOfReviews = new set<JHTSGUID>;
}

// Add a review to the restaurant
JHTSGUID JHTSUser::addReview(int starRating, const string &reviewText, const string &restName, int restZip)
{
    JHTSGUID restaurantGUID = addRestaurant(restName, restZip);
    
    return addReview(starRating, reviewText, restaurantGUID);
}

// Add a review to the restaurant
JHTSGUID JHTSUser::addReview(int starRating, const string &reviewText, JHTSGUID restaurantGUID)
{
    JHTSReview *review = JHTSReview::getReview(mGUID, restaurantGUID);
    
    // If we already have a review, replace it; otherwise create a new one
    if (review != NULL)
        (*review)(starRating, reviewText);
    else
    {
        review = new JHTSReview(starRating, reviewText, mGUID, restaurantGUID);
    
        // Update the user's review cache
        if (mCachedListOfReviews != NULL)
            mCachedListOfReviews->insert(review->guid());
        
        // Update the JHTSRestaurant object's review cache
        JHTSRestaurant *restaurant = JHTSRestaurant::getRestaurant(restaurantGUID);
        
        if (restaurant->mCachedListOfReviews != NULL)
            restaurant->mCachedListOfReviews->insert(review->guid());
    }
    
    return review->guid();
}

// Remove a review to the restaurant
void JHTSUser::deleteReview(JHTSGUID reviewGUID)
{
    JHTSReview *review = JHTSReview::getReview(reviewGUID);
    
    if (review != NULL)
    {
        delete mDatabaseObjectList[reviewGUID];
        mDatabaseObjectList[reviewGUID] = NULL;
    
        // Update the user's review cache
        if (mCachedListOfReviews != NULL)
            mCachedListOfReviews->erase(review->guid());
        
        // Update the JHTSRestaurant object's review cache
        JHTSRestaurant *restaurant = JHTSRestaurant::getRestaurant(review->getRestaurantGUID());
        
        if ((restaurant != NULL) && (restaurant->mCachedListOfReviews != NULL))
            restaurant->mCachedListOfReviews->erase(review->guid());
    }
}

// Add a restaurant
JHTSGUID JHTSUser::addRestaurant(const string &name, int zipCode)
{
    JHTSRestaurant *restaurant = JHTSRestaurant::getRestaurant(name, zipCode);
    
    if (restaurant == NULL)
        restaurant = new JHTSRestaurant(name, zipCode, mGUID);
    
    return restaurant->guid();
}

// Get the list of reviews by this user
set<JHTSGUID> JHTSUser::listOfReviews() const
{
    // If we have an invalid object, recalculate the list
    if (!isValid())
    {
        delete mCachedListOfReviews;
        (const_cast<JHTSUser *>(this))->mCachedListOfReviews = NULL;
    }
    
    // If we have to recalculate the cache
    if (mCachedListOfReviews == NULL)
    {
        set<JHTSGUID> *reviewList = new set<JHTSGUID>;
        JHTSReview *review;

        for (auto ptr = mDatabaseObjectList.begin(); ptr != mDatabaseObjectList.end(); ptr++)
        {
            review = dynamic_cast<JHTSReview *>(*ptr);
            
            if ((review != NULL) && (review->getUserGUID() == mGUID))
                reviewList->insert(review->guid());
        }
        
        (const_cast<JHTSUser *>(this))->mCachedListOfReviews = reviewList;
    }
    
    return *mCachedListOfReviews;
}

// Get the list of users with this name
set<JHTSGUID> JHTSUser::getUsersWithName(const string &lastName, const string &firstName, int zipCode)
{
    set<JHTSGUID> userList = listOfUsers();
    set<JHTSGUID> userListName;
    
    for (auto ptr = userList.begin(); ptr != userList.end(); ptr++)
    {
        JHTSUser *user = getUser(*ptr);
        
        if ((user != NULL) && (user->mLastName == lastName) && (user->mFirstName == firstName))
            if ((zipCode == 0) || (zipCode == user->mZipCode))
                userListName.insert(user->guid());
    }
    
    return userListName;
}

// Get the list of users with this zip code
set<JHTSGUID> JHTSUser::getUsersInZipCode(int zipCode, int numMilesRange)
{
    set<JHTSGUID> userList = listOfUsers();
    set<JHTSGUID> userListZipCode;
    
    for (auto ptr = mDatabaseObjectList.begin(); ptr != mDatabaseObjectList.end(); ptr++)
    {
        JHTSUser *user = dynamic_cast<JHTSUser *>(*ptr);
        
        if ((user != NULL) && (ZipCodeDistance(zipCode, user->mZipCode) <= numMilesRange))
            userListZipCode.insert(user->guid());
    }
    
    return userListZipCode;
}

bool JHTSUser::isValid() const
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
            if (review->getUserGUID() != mGUID)
                return false;
        }
    }

    return true;
}

void JHTSUser::print(ostream &stream) const
{
    set<JHTSGUID> reviewList = listOfReviews();

    stream << "Name: " << mFirstName << " " << mLastName << endl;
    stream << "Zip: " << mZipCode  << endl;
    stream << "Reviews:  ";
    
    for (auto ptr = reviewList.begin(); ptr != reviewList.end(); ptr++)
        stream << (int) *ptr << ", ";
    
    stream << endl;
    stream << "Validity: " << (isValid() ? "true" : "false") << endl;
}
