using System;
using System.Collections.Generic;
using RestaurantReviews.Object;
using RestaurantReviews.Restaurant;
using RestaurantReviews.User;

namespace RestaurantReviews.Review
{
    public class JHTSReview : JHTSObject
    {
        // Properties
        public int UserGUID { get; private set; }
        public int RestaurantGUID { get; private set; }
        public DateTime MostRecentUpdate { get; private set; }

        private int mStarRating;
        public int StarRating
        {
            get => mStarRating;
            set { mStarRating = value; MostRecentUpdate = DateTime.Now; }
        }

        private string mReviewText;
        public string ReviewText
        {
            get => mReviewText;
            set { mReviewText = value; MostRecentUpdate = DateTime.Now; }
        }

        // Internal Constructor
        protected internal JHTSReview(int starRating, string reviewText, int userGUID, int restaurantGUID)
        {
            mStarRating = starRating;
            mReviewText = reviewText;
            UserGUID = userGUID;
            RestaurantGUID = restaurantGUID;
            MostRecentUpdate = DateTime.Now;

            if (mCachedListOfReviews != null)
                mCachedListOfReviews.Add(GUID);

            if (!IsValid)
            {
                clearCache();
                updateCache();
            }
        }

        // Destructor
        ~JHTSReview()
        {
            if (mCachedListOfReviews != null)
                mCachedListOfReviews.Remove(GUID);
        }

        // List of all review objects
        //  (lazy loaded, don't calculate until needed)
        static private SortedSet<int> mCachedListOfReviews = null;
        static public SortedSet<int> ListOfReviews
        {
            get
            {
                if (mCachedListOfReviews == null)
                    mCachedListOfReviews = listByType<JHTSReview>();

                return mCachedListOfReviews;
            }
        }

        // Get a review by guid
        static public JHTSReview getReview(int guid)
        { return (objectAtGUID(guid) as JHTSReview); }

        // Get a review by user/restaurant combination
        static public JHTSReview getReview(int userGUID, int restaurantGUID)
        {
            foreach (var reviewGUID in ListOfReviews)
            {
                JHTSReview review = getReview(reviewGUID);

                if ((review != null) && (review.UserGUID == userGUID) && (review.RestaurantGUID == restaurantGUID))
                    return review;
            }

            return null;
        }

        // Clear the object's cached data
        protected override void clearCache()
        {
            base.clearCache();

            if (mCachedListOfReviews != null)
            {
                mCachedListOfReviews.Clear();
                mCachedListOfReviews = null;
            }
        }

        // Update the object's cached data
        protected override void updateCache()
        {
            base.updateCache();
        }

       // Validity check
        protected override bool IsValid
        {
            get
            {
                if (!base.IsValid)
                    return false;

                // Verify that the associated user exists
                JHTSUser user = JHTSUser.getUser(UserGUID);
                if (user == null)
                    return false;

                // Verify that the associated user has this review
                SortedSet<int> userList = user.ListOfReviews;
                if (!userList.Contains(GUID))
                    return false;

                // Verify that the associated restaurant exists
                JHTSRestaurant restaurant = JHTSRestaurant.getRestaurant(RestaurantGUID);
                if (restaurant == null)
                    return false;

                // Verify that the associated restaurant has this review
                SortedSet<int> restaurantList = restaurant.ListOfReviews;
                if (!restaurantList.Contains(GUID))
                    return false;

                return true;
            }
        }

        // Writing out contents
        public override string ToString()
        {
            string output = base.ToString() + Environment.NewLine;

            output += "Rating: " + StarRating + Environment.NewLine;
            output += "Text: \"" + ReviewText + "\"" + Environment.NewLine;
            output += "by User #" + UserGUID + Environment.NewLine;
            output += "for Restaurant #" + RestaurantGUID + Environment.NewLine;

            return output;
        }
    }
}