#include <map>
#include <iostream>


// A class for representing maps more efficiently. When having consecutive values it stores only the first value. ie 1,A 2,A 3,B 4,B would be stored as 1,A 3,B.
template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest(){};
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	
    

    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
       
            //edge case
            
            if(!(keyBegin < keyEnd)){
                return;
            }

            
        

            //erase
            auto end_it = m_map.upper_bound(keyEnd); //O(log N)
            
            V endvalue;
            if(end_it == m_map.begin()){
                endvalue = m_valBegin;
            } else {
                endvalue = std::prev(end_it)->second;
            }
            
            
            auto start_it = end_it;
            auto step_behind = end_it;
            while(keyBegin < start_it->first && start_it != m_map.begin()){
                start_it = std::prev(start_it);
                if(!(keyBegin < start_it->first)){
                    start_it = std::next(start_it);
                    break;
                } else {
                    start_it = m_map.erase(start_it);
                }

            }

          
            //saving iterators
            int id = 0;
            auto new_start_it = start_it;
            if(start_it != m_map.begin()){
                new_start_it = std::prev(start_it);
            } else {
                id = 1;
            }

            //m_map.erase(start_it,end_it); - we now erase whilst finding start_it so that we're not iterating twice
        
            //edge case
            if(m_map.empty()){
                if(val == m_valBegin){
                    return;
                }
                m_map.emplace_hint(m_map.end(),keyBegin,val);
                m_map.emplace_hint(m_map.end(),keyEnd,m_valBegin);
                return;
            }


            //insert first element
            if(id == 1){
                if(!(val == m_valBegin)){
                    m_map.emplace_hint(std::next(m_map.begin()),keyBegin,val);
                }
            } else if(!(new_start_it->first < keyBegin) && !(keyBegin < new_start_it->first)){
                auto new_new_start_it = std::next(new_start_it);
                m_map.erase(new_start_it);
                if(new_new_start_it == m_map.begin()){
                    if(!(val == m_valBegin)){
                        m_map.emplace_hint(new_new_start_it,keyBegin,val);
                    }
                } else if(!(std::prev(new_new_start_it)->second == val)){
                     m_map.emplace_hint(new_new_start_it,keyBegin,val);
                }
            } else {
                auto after_new_start_it = std::next(new_start_it);
                if(!(new_start_it->second == val)){
                    m_map.emplace_hint(after_new_start_it,keyBegin,val);
                }
            }


            //insert last element
            if(end_it == m_map.end()){
                //cout << "Case 1" << "\n";
                if(end_it != m_map.begin()){
                    if(!(std::prev(end_it)->second == endvalue)){
                        m_map.emplace_hint(m_map.end(),keyEnd,endvalue);
                    }
                }
            } else if(!(end_it->first < keyEnd) && !(keyEnd < end_it->first)){
                //cout << "Case 2" << "\n";
                auto after_end_it = std::next(end_it);
                m_map.erase(end_it);
                if(end_it == m_map.begin()){
                    if(!(endvalue == m_valBegin)){                      
                        if(after_end_it->second == endvalue){
                            m_map.erase(after_end_it);
                        } else{
                            m_map.emplace_hint(after_end_it,keyEnd,endvalue);
                        }
                    }
                } else {
                    if(!(std::prev(end_it)->second == endvalue)){          
                        if(after_end_it->second == endvalue){
                            m_map.erase(after_end_it);
                        } else{
                            m_map.emplace_hint(after_end_it,keyEnd,endvalue);
                        }
                    }
                }
            } else {
                auto after_end_it = std::next(end_it);
                if(!(std::prev(end_it)->second == endvalue)){     
                    if(after_end_it->second == endvalue){
                        m_map.erase(after_end_it);
                    } else{
                         m_map.emplace_hint(after_end_it,keyEnd,endvalue);
                    }
                }
            }
        
    }
        
            
    
	
    
    
    // look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}

    void Test(){
       m_map = {{1,'B'}, {4, 'A'}};
    }
    

    void display(){
        std::cout << "Value Begin: " << m_valBegin << "\n";
        if(m_map.empty()){
            std::cout << "The map is empty." << "\n";
        }
        else{ 
            int n = m_map.size();
            auto it = m_map.begin();
            for (int i=0;i<n;i++){
                std::cout << "(" << it->first << "," << it->second << ")" << "\n";
                advance(it,1);
            }
        }
    }
};

int main(){
    interval_map<int, char> m('A');
    //test.Test();
    m.display();
    
    
    


    
    //m.assign(3, 3, 'B');

   //m.assign(3, 2, 'B');
    
    //m.assign(1, 10, 'B');
  


    //m.assign(1, 3, 'B');
    //m.assign(6, 8, 'C');
    


    //m.assign(std::numeric_limits<int>::lowest(), 10000, 'B');



   //m.assign(std::numeric_limits<int>::lowest(), 10, 'B');
    //m.assign(10, 20, 'B');



   // m.assign(1.2f, 5.f, 'B');


    //m.assign(3, 5, 'B');
   //m.assign(1, 6, 'C');
 


    //m.assign(1, 6, 'C');
    //m.assign(3, 5, 'B');
   

    //m.assign(1, 5, 'B');
    //m.assign(3, 6, 'C');
    


    //m.assign(3, 6, 'C');
    //m.assign(1, 5, 'B');
   


    //m.assign(3, 5, 'B');
    //m.assign(1, 4, 'B');
   


    //m.assign(1, 5, 'B');
    //m.assign(3, 6, 'B');
   


    //m.assign(2, 3, 'B');
    //m.assign(1, 5, 'B');
    


    //m.assign(2, 5, 'B');
    //m.assign(5, 8, 'C');
    //m.assign(4, 5, 'A');

 
    //m.assign(1, 5, 'B');
    //m.assign(2, 3, 'B');
   


    //m.assign(1, 5, 'B');
    //m.assign(4, 6, 'A');



    //m.assign(1, 5, 'B');
    //m.assign(1, 5, 'A');
    


    //m.assign(1, 5, 'B');
    //m.assign(0, 7, 'A');
   


    m.assign(3, 6, 'B');
    m.assign(2, 5, 'C');
    m.assign(4, 7, 'A');
    m.display();
    return 0;
}