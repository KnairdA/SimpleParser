{ stdenv, fetchFromGitHub, cmake, boost, gtest }:

stdenv.mkDerivation rec {
  name = "SimpleParser";

  src = fetchFromGitHub {
    owner  = "KnairdA";
    repo   = "SimpleParser";
    rev    = "v1.1";
    sha256 = "1i38nrb28g1nzhcs0139arz8vvbnli9ryvdl657dqslb6ms1x0g2";
  };

  buildInputs = [ cmake boost gtest ];

  meta = with stdenv.lib; {
    homepage = https://github.com/KnairdA/SimpleParser/;
  };
}
