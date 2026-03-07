{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE MultiParamTypeClasses #-}
{-# LANGUAGE OverlappingInstances #-}
{-# LANGUAGE RankNTypes #-}
{-# LANGUAGE ScopedTypeVariables #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE UndecidableInstances #-}

-- Vendored copy of mtl-evil-instances-0.1, patched for GHC 9.x / mtl-2.3.
--
-- mtl-2.3 added explicit superclass constraints (Monad m, Monoid w) to
-- several type classes.  All instance declarations that derive from those
-- classes must now carry those constraints in their context.
--
-- Changes vs upstream 0.1:
--   MonadFix   instances: + Monad m
--   MonadIO    instances: + Monad m
--   MonadReader instances: + Monad m
--   MonadRWS   instances: + Monad m, Monoid w
--   MonadState instances: + Monad m
--   MonadWriter instances: + Monad m, Monoid w
--
-- DeMoD LLC vendor patch — GPL-3.0 (same as upstream)

module Control.Monad.Instances.Evil
    ()
where

import           Control.Applicative (Applicative (..))
import           Control.Monad.Base (MonadBase (..))
import           Control.Monad.Cont.Class (MonadCont(..))
import           Control.Monad.Error.Class (MonadError (..))
import           Control.Monad.Fix (MonadFix (..), fix)
import           Control.Monad.IO.Class (MonadIO (..))
import           Control.Monad.Reader.Class (MonadReader (..))
import           Control.Monad.RWS.Class (MonadRWS (..))
import           Control.Monad.State.Class (MonadState (..))
import           Control.Monad.Trans.Class (MonadTrans (..))
import           Control.Monad.Trans.Control
                     ( ComposeSt
                     , MonadBaseControl (..)
                     , MonadTransControl (..)
                     , Run
                     , control
                     , defaultLiftBaseWith
                     , defaultRestoreM
                     )
import           Control.Monad.Writer.Class (MonadWriter (..))


------------------------------------------------------------------------------
instance (MonadTrans t, Applicative (t m), Monad (t m), MonadBase b m) => MonadBase b (t m) where
    liftBase = lift . liftBase


------------------------------------------------------------------------------
instance (MonadTransControl t, Monad (t m), MonadCont m) => MonadCont (t m) where
    callCC f = controlT $ \run -> callCC $ \c -> run . f $
        \a -> lift (run (return a) >>= c)


------------------------------------------------------------------------------
-- mtl-2.3: MonadCont gains `Monad m` superclass → must be in context
instance (Monad m, MonadBaseControl b m, MonadCont b) => MonadCont m where
    callCC f = control $ \run -> callCC $ \c -> run . f $
        \a -> liftBase (run (return a) >>= c)


------------------------------------------------------------------------------
instance (MonadTransControl t, Monad (t m), MonadError e m) => MonadError e (t m) where
    throwError = lift . throwError
    catchError t h = controlT $ \run -> catchError (run t) (\e -> run (h e))


------------------------------------------------------------------------------
-- mtl-2.3: MonadError gains `Monad m` superclass → must be in context
instance (Monad m, MonadBaseControl b m, MonadError e b) => MonadError e m where
    throwError = liftBase . throwError
    catchError t h = control $ \run -> catchError (run t) (\e -> run (h e))


------------------------------------------------------------------------------
-- mtl-2.3: MonadFix gains `Monad m` superclass → must be in context
instance (MonadTransControl t, Monad (t m), MonadFix m) => MonadFix (t m) where
    mfix f = controlT $ \run -> mfix (\a -> run (restoreT (return a) >>= f))


------------------------------------------------------------------------------
-- mtl-2.3: MonadFix gains `Monad m` superclass → must be in context
instance (Monad m, MonadBaseControl b m, MonadFix b) => MonadFix m where
    mfix f = control $ \run -> mfix (\a -> run (restoreM a >>= f))


------------------------------------------------------------------------------
-- mtl-2.3: MonadIO gains `Monad m` superclass → must be in context
instance (MonadTrans t, Monad (t m), MonadIO m) => MonadIO (t m) where
    liftIO = lift . liftIO


------------------------------------------------------------------------------
-- mtl-2.3: MonadIO gains `Monad m` superclass → must be in context
instance (Monad m, MonadBase b m, MonadIO b) => MonadIO m where
    liftIO = liftBase . liftIO


------------------------------------------------------------------------------
-- mtl-2.3: MonadReader gains `Monad m` superclass → must be in context
instance (MonadTrans t, Monad (t m), MonadReader r m) => MonadReader r (t m) where
    ask   = lift ask
    local f m = m >>= lift . local f . return


------------------------------------------------------------------------------
-- mtl-2.3: MonadReader gains `Monad m` superclass → must be in context
instance (Monad m, MonadBase b m, MonadReader r b) => MonadReader r m where
    ask   = liftBase ask
    local f m = m >>= liftBase . local f . return


------------------------------------------------------------------------------
-- mtl-2.3: MonadRWS gains `Monad m, Monoid w` superclasses → must be in context
instance (Monad (t m), MonadTrans t, Monad (t m), MonadRWS r w s m) => MonadRWS r w s (t m)


------------------------------------------------------------------------------
-- mtl-2.3: MonadRWS gains `Monad m, Monoid w` superclasses → must be in context
instance (Monoid w, Monad m, MonadBase b m, MonadRWS r w s b) => MonadRWS r w s m


------------------------------------------------------------------------------
-- mtl-2.3: MonadState gains `Monad m` superclass → must be in context
instance (MonadTrans t, Monad (t m), MonadState s m) => MonadState s (t m) where
    get   = lift get
    put s = lift $ put s


------------------------------------------------------------------------------
-- mtl-2.3: MonadState gains `Monad m` superclass → must be in context
instance (Monad m, MonadBase b m, MonadState s b) => MonadState s m where
    get   = liftBase get
    put s = liftBase $ put s


------------------------------------------------------------------------------
-- mtl-2.3: MonadWriter gains `Monad m, Monoid w` superclasses → must be in context
instance (MonadTrans t, Monad (t m), MonadWriter w m) => MonadWriter w (t m) where
    tell w  = lift $ tell w
    listen m = m >>= lift . listen . return
    pass   m = m >>= lift . pass . return


------------------------------------------------------------------------------
-- mtl-2.3: MonadWriter gains `Monad m, Monoid w` superclasses → must be in context
instance (Monoid w, Monad m, MonadBase b m, MonadWriter w b) => MonadWriter w m where
    tell w  = liftBase $ tell w
    listen m = m >>= liftBase . listen . return
    pass   m = m >>= liftBase . pass . return


------------------------------------------------------------------------------
controlT
    :: (MonadTransControl t, Monad (t m), Monad m)
    => (Run t -> m (StT t a)) -> t m a
controlT f = liftWith f >>= restoreT . return
