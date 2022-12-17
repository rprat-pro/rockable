//  Copyright or © or Copr. Rockable
//
//  vincent.richefeu@3sr-grenoble.fr
//
//  This software is a computer program whose purpose is
//    (i)  to hold sphero-polyhedral shapes,
//    (ii) to manage breakable interfaces.
//  It is developed for an ACADEMIC USAGE
//
//  This software is governed by the CeCILL-B license under French law and
//  abiding by the rules of distribution of free software.  You can  use,
//  modify and/ or redistribute the software under the terms of the CeCILL-B
//  license as circulated by CEA, CNRS and INRIA at the following URL
//  "http://www.cecill.info".
//
//  As a counterpart to the access to the source code and  rights to copy,
//  modify and redistribute granted by the license, users are provided only
//  with a limited warranty  and the software's author,  the holder of the
//  economic rights,  and the successive licensors  have only  limited
//  liability.
//
//  In this respect, the user's attention is drawn to the risks associated
//  with loading,  using,  modifying and/or developing or reproducing the
//  software by the user in light of its specific status of free software,
//  that may mean  that it is complicated to manipulate,  and  that  also
//  therefore means  that it is reserved for developers  and  experienced
//  professionals having in-depth computer knowledge. Users are therefore
//  encouraged to load and test the software's suitability as regards their
//  requirements in conditions enabling the security of their systems and/or
//  data to be ensured and,  more generally, to use and operate it in the
//  same conditions as regards security.
//
//  The fact that you are presently reading this means that you have had
//  knowledge of the CeCILL-B license and that you accept its terms.

#include "SpringJoint.hpp"

SpringJoint::SpringJoint(size_t ibody_, size_t jbody_, vec3r& ipos0_, vec3r& jpos0_)
    : ibody(ibody_), jbody(jbody_), ipos0(ipos0_), jpos0(jpos0_) {}

SpringJoint::~SpringJoint() {}

void SpringJoint::init(std::vector<Particle>& particles, double stiffness) {
  vec3r ipos = particles[ibody].pos + particles[ibody].Q * ipos0;
  vec3r jpos = particles[jbody].pos + particles[jbody].Q * jpos0;
  // std::cout << "ipos0 = " << ipos0 << '\n';
  // std::cout << "jpos0 = " << jpos0 << '\n';
  // std::cout << "ipos = " << ipos << '\n';
  // std::cout << "jpos = " << jpos << '\n';
  l0 = (jpos - ipos).normalize();
  // std::cout << "l0 = " << l0 << '\n';
  K = fabs(stiffness);
}

void SpringJoint::getForceOnj(std::vector<Particle>& particles, vec3r& force) {
  vec3r ipos = particles[ibody].pos + particles[ibody].Q * ipos0;
  vec3r jpos = particles[jbody].pos + particles[jbody].Q * jpos0;
  vec3r n = (jpos - ipos);
  double delta_length = n.normalize() - l0;
  force = (-K * delta_length) * n;
}
