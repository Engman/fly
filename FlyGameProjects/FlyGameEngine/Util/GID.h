#ifndef GID_H
#define GID_H

#include <vector>

/** 
*	This class only purpos is to generate a uniqe global id, nothing else.. 
*/
class GID
{
	private:
		int id;	/**< A uniuqe application identity */
		int usft()
		{
			static int ID = 0;
			return ID++;
		}

	public:
		/** Creates a uniuqe identity using static id counter */
		GID();
		virtual~GID();

#if defined(_DEBUG) || (DEBUG)
		static std::vector<int>& getAllocList();
#endif

		GID(const GID& o)
		{
			this->id = usft();
		}
		const GID& operator=(const GID& o)
		{
			this->id = usft();
			return *this;
		}
		operator int() const
		{
			return this->id;
		}
		bool operator == (const GID& object)
		{
			return this->id == object.id;
		}


		/** Returns the unique identity */
		int get() const { return id; }
};


#endif

