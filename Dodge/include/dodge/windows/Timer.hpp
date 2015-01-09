/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TIMER_HPP__
#define __TIMER_HPP__


namespace Dodge {


class Timer {
   public:
      Timer() { reset(); }

      void reset();
      double getTime() const;

   private:
      double m_seconds;
};


}


#endif /*!__TIMER_HPP__*/
