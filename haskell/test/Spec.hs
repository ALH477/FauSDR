-- test/Spec.hs — unified test runner for dcf-faust-sdr
-- DeMoD LLC
--
-- Run: cabal test frame-tests --test-show-details=direct

import Test.Hspec
import qualified FrameSpec
import qualified ConformanceSpec

main :: IO ()
main = hspec $ do
  describe "Frame + Symbol"          FrameSpec.spec
  describe "Cross-language conformance" ConformanceSpec.spec
