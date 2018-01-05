using System;
using System.Collections.Generic;

namespace RestaurantReviews.Object
{
    public class JHTSObject
    {
        // Properties
        public int GUID { get; private set; }
        public DateTime CreationTime { get; private set; }
        static private List<JHTSObject> mObjectList = new List<JHTSObject>();
        static protected List<JHTSObject> ObjectList
        { get { return mObjectList; } }

        // Constructor
        public JHTSObject()
        {
            CreationTime = DateTime.Now;
            GUID = ObjectList.Count;
            ObjectList.Add(this);
        }

        // Destructor
        ~JHTSObject()
        {
            if ((GUID >= 0) && (GUID < ObjectList.Count))
                ObjectList[GUID] = null;
        }

        // The master list of allocated objects
        static public JHTSObject objectAtGUID(int guid)
        {
            if ((guid < 0) || (ObjectList == null) || (guid > ObjectList.Count))
                return null;

            return ObjectList[guid];
        }

        // Generic method to return a sublist by type
        static protected SortedSet<int> listByType<SubClassType>()
        {
            SortedSet<int> subClassList = new SortedSet<int>();

            foreach (var item in ObjectList)
                if (item is SubClassType)
                    subClassList.Add(item.GUID);

            return subClassList;
        }

        // Normally, we would import a location library that can give real
        //  distances between two zip codes, but for the purposes of this
        //  exercise, we will cheat, and assume that the zipCode increments
        //  or decrements with each mile.
        static public int ZipCodeDistance(int zipCode1, int zipCode2)
        { return Math.Abs(zipCode1 - zipCode2); }

        // Clear the object's cached data
        //  (used by subclasses)
        protected virtual void clearCache()
        { }

        // Update the object's cached data
        //  (used by subclasses)
        protected virtual void updateCache()
        { }

        // Validity check
        protected virtual bool IsValid
        { get { return (GUID >= 0) && (GUID < ObjectList.Count); } }

        // Writing out contents
        public override string ToString()
        {
            string output = "Object #" + GUID + " of type: " + GetType().Name + Environment.NewLine;

            if (!IsValid)
            {
                output += "***************************" + Environment.NewLine;
                output += "    Fails Validity Test!" + Environment.NewLine;
                output += "***************************" + Environment.NewLine;
            }

            return output;
        }

        public static void printObjects()
        {
            int kMaxNum = (int) JHTSObject.ObjectList.Count;

            if (kMaxNum < 1)
            {
                Console.WriteLine("ObjectList size = " + kMaxNum);
                return;
            }

            JHTSObject item;

            for (int index = 0; index < kMaxNum; index++)
            {
                item = ObjectList[index];

                Console.WriteLine("*************************");
                Console.WriteLine("INDEX #" + index);
                if (item == null)
                    Console.WriteLine("null");
                else
                    Console.WriteLine(item);
                Console.WriteLine("*************************");
            }
        }
    }
}