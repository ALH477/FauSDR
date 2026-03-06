-- test/Spec.hs — unified test runner for dcf-faust-sdr
-- DeMoD LLC
--
-- Run: cabal test frame-tests --test-show-details=direct

import Test.Hspec
import qualified FrameSpec

main :: IO ()
main = hspec $ do
  describe "Frame + Symbol" FrameSpec.spec
