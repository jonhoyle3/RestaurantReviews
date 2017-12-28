#ifndef JHTSAPI_H
#define JHTSAPI_H
//
//  JHTSAPI.h
//  API
//
//  Creates a C-API thunk layer, so that other development
//  environments (eg: Java using JNI) can call into this
//  library of code.
//
//  For simplicity, bools (rather than full error codes)
//   are returned to determine if the call succeeded or
//   failed.
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

typedef uint64_t JHTSGUID;

#ifdef __cplusplus
    extern "C"
    {
#endif


// JHTSUser
size_t NumJHTSUsers();
bool GetListOfJHTSUsers(JHTSGUID userHandles[], size_t userHandleArraySize);
JHTSGUID CreateNewJHTSUser(const char *lastName, const char *firstName, int zipCode);
JHTSGUID CreateNewJHTSUserWithZipCode(const char *lastName, const char *firstName);

bool GetJHTSUserLastName(JHTSGUID userHandle, char lastName[], size_t lastNameBufferSize);
bool GetJHTSUserFirstName(JHTSGUID userHandle, char firstName[], size_t lastNameBufferSize);
int GetJHTSUserZipCode(JHTSGUID userHandle);
bool GetJHTSUserHandleByName(const char *lastName, const char *firstName, int zipCode, JHTSGUID userHandles[], size_t userHandleArraySize);


// JHTSRestaurant
size_t NumJHTSRestaurants();
bool GetListOfJHTSRestaurants(JHTSGUID restaurantHandles[], size_t restaurantHandleArraySize);

bool GetJHTSRestaurantName(JHTSGUID restaurantHandle, char restaurantName[], size_t nameBufferSize);
int GetJHTSRestaurantZipCode(JHTSGUID restaurantHandle);
JHTSGUID GetJHTSRestaurantCreator(JHTSGUID restaurantHandle);
int GetJHTSRestaurantAverageStarRating(JHTSGUID restaurantHandle);

bool SetJHTSRestaurantName(JHTSGUID restaurantHandle, const char *updatedRestaurantName);
bool SetJHTSRestaurantZipCode(JHTSGUID restaurantHandle, int updatedRestaurantZip);

size_t NumJHTSReviewsForJHTRestaurant(JHTSGUID restaurantHandle);
bool GetListOfJHTSReviewsForJHTRestaurant(JHTSGUID restaurantHandle, JHTSGUID reviewHandles[], size_t reviewHandleArraySize);
bool GetListOfJHTSRestaurantsByName(const char *restaurantName, JHTSGUID restaurantHandles[], size_t restaurantHandleArraySize);
bool GetListOfJHTSRestaurantsInRange(int zipCode, int milesRadius, JHTSGUID restaurantHandles[], size_t restaurantHandleArraySize);
bool GetListOfJHTSRestaurantsInRangeWithMinRating(int zipCode, int milesRadius, int minStarRating, JHTSGUID restaurantHandles[], size_t restaurantHandleArraySize);


// JHTSReviews
size_t NumJHTSReviews();
bool GetListOfJHTSReviews(JHTSGUID reviewHandles[], size_t reviewHandleArraySize);

JHTSGUID GetJHTSUserForJHTSReview(JHTSGUID reviewHandle);
JHTSGUID GetJHTSResaturantForJHTSReview(JHTSGUID reviewHandle);
int GetStarRatingForJHTSReview(JHTSGUID reviewHandle);
bool GetTextForJHTSReview(JHTSGUID reviewHandle, char reviewText[], size_t reviewBufferSize);


// JHTSUser managing reviews
size_t NumJHTSReviewsForJHTSUser(JHTSGUID userHandle);
bool GetListOfJHTSReviewsForJHTUser(JHTSGUID userHandle, JHTSGUID reviewHandles[], size_t reviewHandleArraySize);
JHTSGUID CreateJHTSReview(JHTSGUID userHandle, JHTSGUID restaurantHandle, int starRating, const char *reviewText);
bool DeleteJHTSReview(JHTSGUID userHandle, JHTSGUID reviewHandle);

bool SetStarRatingForJHTSReview(JHTSGUID userHandle, JHTSGUID reviewHandle, int updatedStarRating);
bool SetTextForJHTSReview(JHTSGUID userHandle, JHTSGUID reviewHandle, const char *updatedReviewText);


// JHTSUser managing restaurants
JHTSGUID CreateNewJHTSRestaurant(JHTSGUID userHandle, const char *restaurantName, int zipCode);
size_t NumJHTSRestaurantsForJHTSUser(JHTSGUID userHandle);
bool GetListOfJHTSRestaurantsForJHTUser(JHTSGUID userHandle, JHTSGUID restaurantHandles[], size_t restaurantHandleArraySize);


#ifdef __cplusplus
    }
#endif

#endif // JHTSAPI_H
