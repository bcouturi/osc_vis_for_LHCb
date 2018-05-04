// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef AIDA_IBRUSHSTYLE_H
#define AIDA_IBRUSHSTYLE_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <string>
#include <vector>

#include "AIDA/IBaseStyle.h"

namespace AIDA {

/**
 * Superclass for anything which can be drawn in different 
 * colours and with (possibly) transparent/opaque/translucent effect.
 *
 * @author The AIDA team (http://aida.freehep.org/)
 */

class IBrushStyle : virtual public IBaseStyle {

public: 
    /// Destructor.
    virtual ~IBrushStyle() { /* nop */; }

  /**
   * Get a list of the colours supported by this implementation.
   */
    virtual std::vector<std::string>  availableColors() const = 0;

  /** 
   * Current colour of this brush style.
   */
    virtual std::string color() const = 0;

  /** 
   * Current opacity (alpha) of this brush style.
   */
    virtual double opacity() const = 0;

  /** 
   * Set current colour, e.g. "darkGreen"
   * @return false if colour not recognised by the implementation.
   */
    virtual bool setColor(const std::string & newColor) = 0;

  /**
   * Set opacity (alpha), e.g. 0 (transparent), 
   * 1 (opaque), 0.5 (halfway).
   * @return false if value not supported by the implementation.
   */
    virtual bool setOpacity(double newOpacity) = 0;
}; // class
} // namespace AIDA
#endif /* ifndef AIDA_IBRUSHSTYLE_H */
