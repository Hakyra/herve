// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_SOUNDFXREVERB_HPP_
#define HERVE_SOUNDFXREVERB_HPP_

#include <Herve/SoundFx.hpp>

namespace Herve {

/// \brief simple reverb sound effect
///
class SoundFxReverb : public SoundFx {
  EFXEAXREVERBPROPERTIES _reverbProperties;
public:
  /// \brief init effect with default parameters
  ///
  void init();
};

} // namespace Herve

#endif


