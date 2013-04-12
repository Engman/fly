#ifndef GID_H
#define GID_H

/** 
*	This class only purpos is to generate a uniqe global id, nothing else.. 
*/
class GID
{
	private:
		int id;	/**< A uniuqe application identity */

	public:
		/** Creates a uniuqe identity using static id counter */
		GID() 
		{ 
			static int ID = 1; 
			this->id = ID;
			ID++;
		}  
		/** Returns the unique identity */
		int get() const { return id; }
		operator int() const
		{
			return this->id;
		}
		bool operator == (const GID& object)
		{
			return this->id == object.id;
		}
};


#endif

