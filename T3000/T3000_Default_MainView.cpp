/ /   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w . c p p   :   i m p l e m e n t a t i o n   f i l e  
 / /  
  
 # i n c l u d e   " s t d a f x . h "  
 # i n c l u d e   " T 3 0 0 0 . h "  
 # i n c l u d e   " T 3 0 0 0 _ D e f a u l t _ M a i n V i e w . h "  
 # i n c l u d e   " M a i n F r m . h "  
    
  
 / /   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w  
  
 I M P L E M E N T _ D Y N C R E A T E ( T 3 0 0 0 _ D e f a u l t _ M a i n V i e w ,   C F o r m V i e w )  
  
 T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : T 3 0 0 0 _ D e f a u l t _ M a i n V i e w ( )  
 	 :   C F o r m V i e w ( T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : I D D )  
 {  
  
 }  
  
 T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : ~ T 3 0 0 0 _ D e f a u l t _ M a i n V i e w ( )  
 {  
 }  
  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : D o D a t a E x c h a n g e ( C D a t a E x c h a n g e *   p D X )  
 {  
         C F o r m V i e w : : D o D a t a E x c h a n g e ( p D X ) ;  
         D D X _ C o n t r o l ( p D X ,   I D C _ I P M O D E L _ C O M B O ,   m _ i p _ m o d e l ) ;  
         D D X _ C o n t r o l ( p D X ,   I D C _ I P A D D R E S S 1 ,   m _ i p a d d r e s s ) ;  
         D D X _ C o n t r o l ( p D X ,   I D C _ I P A D D R E S S 2 ,   m _ S u b n e t _ M a s k ) ;  
         D D X _ C o n t r o l ( p D X ,   I D C _ I P A D D R E S S 3 ,   m _ G a t e w a y A d d r e s s ) ;  
         D D X _ C o n t r o l ( p D X ,   I D C _ M A C _ A D D R E S S ,   m _ E d i t _ M a c A d d r e s s ) ;  
         D D X _ C o n t r o l ( p D X ,   I D C _ E D I T _ L I S T E N P O R T ,   m _ L i s t e n P o r t ) ;  
 }  
  
 B E G I N _ M E S S A G E _ M A P ( T 3 0 0 0 _ D e f a u l t _ M a i n V i e w ,   C F o r m V i e w )  
 	 O N _ B N _ C L I C K E D ( I D C _ E D I T _ I D ,   & T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n B n C l i c k e d E d i t I d )  
         O N _ B N _ C L I C K E D ( I D C _ B U T T O N _ G O _ T O _ I N P U T ,   & T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n B n C l i c k e d B u t t o n G o T o I n p u t )  
         O N _ B N _ C L I C K E D ( I D C _ A P P L Y B U T T O N ,   & T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n B n C l i c k e d A p p l y b u t t o n )  
         O N _ C B N _ S E L C H A N G E ( I D C _ I P M O D E L _ C O M B O ,   & T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n C b n S e l c h a n g e I p m o d e l C o m b o )  
 E N D _ M E S S A G E _ M A P ( )  
  
  
 / /   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w   d i a g n o s t i c s  
  
 # i f d e f   _ D E B U G  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : A s s e r t V a l i d ( )   c o n s t  
 {  
 	 C F o r m V i e w : : A s s e r t V a l i d ( ) ;  
 }  
  
 # i f n d e f   _ W I N 3 2 _ W C E  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : D u m p ( C D u m p C o n t e x t &   d c )   c o n s t  
 {  
 	 C F o r m V i e w : : D u m p ( d c ) ;  
 }  
 # e n d i f  
 # e n d i f   / / _ D E B U G  
  
  
 / /   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w   m e s s a g e   h a n d l e r s  
  
  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n I n i t i a l U p d a t e ( )  
 {  
 	 C F o r m V i e w : : O n I n i t i a l U p d a t e ( ) ;  
  
 	 / /   T O D O :   A d d   y o u r   s p e c i a l i z e d   c o d e   h e r e   a n d / o r   c a l l   t h e   b a s e   c l a s s  
 }  
  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : F r e s h ( ) {  
         C S t r i n g   s t r T e m p ;  
 	 s t r T e m p . F o r m a t ( _ T ( " % d " ) , g _ t s t a t _ i d ) ;  
 	 G e t D l g I t e m ( I D C _ I D _ E D I T ) - > S e t W i n d o w T e x t W ( s t r T e m p ) ;  
 	 s t r T e m p . F o r m a t ( _ T ( " % d " ) , g e t _ s e r i a l n u m b e r ( ) ) ;  
 	 G e t D l g I t e m ( I D C _ S E R I A L N U M _ E D I T ) - > S e t W i n d o w T e x t W ( s t r T e m p ) ;  
  
 	 s t r T e m p . F o r m a t ( _ T ( " % 0 . 1 f " ) , g e t _ c u r t s t a t _ v e r s i o n ( ) ) ;  
 	 G e t D l g I t e m ( I D C _ F I R M W A R V _ E D I T ) - > S e t W i n d o w T e x t W ( s t r T e m p ) ;  
 	 s t r T e m p . F o r m a t ( _ T ( " % d " ) , p r o d u c t _ r e g i s t e r _ v a l u e [ 8 ] ) ; / / 8  
 	 G e t D l g I t e m ( I D C _ H A R D W A R E V _ E D I T ) - > S e t W i n d o w T e x t W ( s t r T e m p ) ;  
 	 i n t   n M o d e l = p r o d u c t _ r e g i s t e r _ v a l u e [ 7 ] ;  
 	 G e t D l g I t e m ( I D C _ M O D E L _ E D I T ) - > S e t W i n d o w T e x t W ( G e t P r o d u c t N a m e ( n M o d e l ) ) ;  
 	 s t r T e m p . F o r m a t ( _ T ( " % d " ) , p r o d u c t _ r e g i s t e r _ v a l u e [ 9 ] ) ;  
 	 G e t D l g I t e m ( I D C _ P I C _ E D I T ) - > S e t W i n d o w T e x t ( s t r T e m p ) ;  
  
         i f   ( p r o d u c t _ r e g i s t e r _ v a l u e [ 7 ]   = =   P M _ T 3 2 2 A I   | |   p r o d u c t _ r e g i s t e r _ v a l u e [ 7 ]   = =   P M _ T 3 8 A I 8 A O 6 D O )  
         {  
                 C S t r i n g   s t r t e m p ;  
                 i n t   C O 2 _ N E T _ M O D B U S _ M A C _ A D D R E S S _ S T A R T   =   4 0 ;  
                 s t r t e m p . F o r m a t ( _ T ( " % 0 2 X - % 0 2 X - % 0 2 X - % 0 2 X - % 0 2 X - % 0 2 X " ) , p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ M A C _ A D D R E S S _ S T A R T ] , p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ M A C _ A D D R E S S _ S T A R T + 1 ] , p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ M A C _ A D D R E S S _ S T A R T + 2 ] , p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ M A C _ A D D R E S S _ S T A R T + 3 ] , p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ M A C _ A D D R E S S _ S T A R T + 4 ] , p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ M A C _ A D D R E S S _ S T A R T + 5 ] ) ;  
                 m _ E d i t _ M a c A d d r e s s . S e t W i n d o w T e x t ( s t r t e m p ) ;  
  
                 i n t   C O 2 _ N E T _ M O D B U S _ I P _ M O D E   =   4 6 ;  
                 m _ i p _ m o d e l . S e t C u r S e l ( p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ I P _ M O D E ] ) ;  
                 / / C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ S T A R T  
                 / / C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ S T A R T  
                 / / C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ S T A R T  
                 / / C O 2 _ N E T _ M O D B U S _ T C P _ S E R V E R _ E N A B L E  
                 / / C O 2 _ N E T _ M O D B U S _ L I S T E N _ P O R T _ A T _ T C P _ S E R V E R _ M O D E _ S T A R T  
                 i n t       C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ S T A R T   =   4 7 ;  
                 m _ i p a d d r e s s . S e t A d d r e s s ( ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ S T A R T ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ S T A R T + 1 ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ S T A R T + 2 ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ S T A R T + 3 ] ) ;  
                   i n t       C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ S T A R T     =           5 1 ;  
                 m _ S u b n e t _ M a s k . S e t A d d r e s s ( ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ S T A R T ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ S T A R T + 1 ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ S T A R T + 2 ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ S T A R T + 3 ] ) ;  
                           i n t       C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ S T A R T     =           5 5 ;  
  
                 m _ G a t e w a y A d d r e s s . S e t A d d r e s s ( ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ S T A R T ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ S T A R T + 1 ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ S T A R T + 2 ] ,  
                         ( B Y T E ) p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ S T A R T + 3 ] ) ;  
                     i n t   C O 2 _ N E T _ M O D B U S _ L I S T E N _ P O R T _ A T _ T C P _ S E R V E R _ M O D E _ S T A R T   =     6 0 ;  
                 s t r t e m p . F o r m a t ( _ T ( " % d " ) , p r o d u c t _ r e g i s t e r _ v a l u e [ C O 2 _ N E T _ M O D B U S _ L I S T E N _ P O R T _ A T _ T C P _ S E R V E R _ M O D E _ S T A R T ] ) ;  
                 m _ L i s t e n P o r t . S e t W i n d o w T e x t ( s t r t e m p ) ;  
         }  
  
  
 }  
 # i n c l u d e   " E r e a s e D l g . h "  
  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n B n C l i c k e d E d i t I d ( )  
 {  
 	   C E r e a s e D l g   d l g ;  
 	   d l g . D o M o d a l ( ) ;  
  
           C S t r i n g   s t r ;  
           s t r . F o r m a t   ( _ T ( " % d " ) , g _ t s t a t _ i d ) ;  
  
           G e t D l g I t e m   ( I D C _ I D _ E D I T ) - > S e t W i n d o w T e x t W ( s t r ) ;  
 }  
  
  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n B n C l i c k e d B u t t o n G o T o I n p u t ( )  
 {  
           / *   i f   ( p r o d u c t _ t y p e   = =   T 3 0 0 0 _ T 3 _ M O D U L E S   )  
           {  
                   S w i t c h T o P r u d u c t T y p e ( D L G _ D I A L O G _ T 3 _ I N P U T S _ V I E W ) ;  
         } * /  
         C M a i n F r a m e *   p F r a m e = ( C M a i n F r a m e * ) ( A f x G e t A p p ( ) - > m _ p M a i n W n d ) ;  
         p F r a m e - > O n C o n t r o l I n p u t s   ( ) ;  
 }  
  
  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n B n C l i c k e d A p p l y b u t t o n ( )  
 {  
         i n t   m _ n L i s t e n P o r t = 0 ;  
         B e g i n W a i t C u r s o r ( ) ;  
         / / m _ n L i s t e n P o r t 	  
         C S t r i n g   s t r T e x t ;  
         m _ L i s t e n P o r t . G e t W i n d o w T e x t ( s t r T e x t ) ;  
         i f ( ! s t r T e x t . I s E m p t y ( ) )  
                 m _ n L i s t e n P o r t = _ w t o i ( s t r T e x t ) ;  
         i f ( m _ n L i s t e n P o r t > = 1 2 7 6 7 )  
         {  
                 A f x M e s s a g e B o x ( _ T ( " T h e   l i s t e n   p o r t   n u m b e r   i s   t o o   b i g ,   p l e a s e   c h a n g e   i t . " ) ) ;  
                 r e t u r n ;  
         }  
         i f ( m _ n L i s t e n P o r t < = 0 )  
         {  
                 A f x M e s s a g e B o x ( _ T ( " T h e   l i s t e n   p o r t   n u m b e r   m u s t   b e   g r e a t e r   t h a n   0 ! " ) ) ;  
                 r e t u r n ;  
         }  
         w r i t e _ o n e ( g _ t s t a t _ i d , 7 5 , m _ n L i s t e n P o r t ) ;  
         p r o d u c t _ r e g i s t e r _ v a l u e [ 7 5 ] = m _ n L i s t e n P o r t ;  
         B Y T E   a d d r e s s 1 , a d d r e s s 2 , a d d r e s s 3 , a d d r e s s 4 ;  
         m _ i p a d d r e s s . G e t A d d r e s s ( a d d r e s s 1 , a d d r e s s 2 , a d d r e s s 3 , a d d r e s s 4 ) ;  
  
         i n t   s e l   =   m _ i p _ m o d e l . G e t C u r S e l   ( ) ;  
           w r i t e _ o n e ( g _ t s t a t _ i d , 6 1 , s e l ) ;  
  
         i n t   C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ G H O S T _ S T A R T   =   6 2   ;  
         i n t   n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ G H O S T _ S T A R T , a d d r e s s 1 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ G H O S T _ S T A R T + 1 , a d d r e s s 2 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ G H O S T _ S T A R T + 2 , a d d r e s s 3 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ I P _ A D D R E S S _ G H O S T _ S T A R T + 3 , a d d r e s s 4 ) ;  
         i n t   C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ G H O S T _ S T A R T   =   6 6 ;  
         m _ S u b n e t _ M a s k . G e t A d d r e s s ( a d d r e s s 1 , a d d r e s s 2 , a d d r e s s 3 , a d d r e s s 4 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ G H O S T _ S T A R T + 0 , a d d r e s s 1 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ G H O S T _ S T A R T + 1 , a d d r e s s 2 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ G H O S T _ S T A R T + 2 , a d d r e s s 3 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ S U B N E T _ M A S K _ A D D R E S S _ G H O S T _ S T A R T + 3 , a d d r e s s 4 ) ;  
         m _ G a t e w a y A d d r e s s . G e t A d d r e s s ( a d d r e s s 1 , a d d r e s s 2 , a d d r e s s 3 , a d d r e s s 4 ) ;  
         i n t   C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ G H O S T _ S T A R T   =       7 0 ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ G H O S T _ S T A R T , a d d r e s s 1 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ G H O S T _ S T A R T + 1 , a d d r e s s 2 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ G H O S T _ S T A R T + 2 , a d d r e s s 3 ) ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ G A T E W A Y _ A D D R E S S _ G H O S T _ S T A R T + 3 , a d d r e s s 4 ) ;  
         i n t   C O 2 _ N E T _ M O D B U S _ E N A B L E _ G H O S T   =   7 6 ;  
         n = w r i t e _ o n e ( g _ t s t a t _ i d , C O 2 _ N E T _ M O D B U S _ E N A B L E _ G H O S T , 1 ) ; / / � � � � � � � �  
  
         S l e e p ( 5 * 1 0 0 0 ) ;  
  
  
  
         C M a i n F r a m e *   p P r a e n t = ( C M a i n F r a m e * ) G e t P a r e n t ( ) ;  
  
         C S t r i n g   s t r B u i l d i n g , s t r S u b B u i l d i n g ;  
         s t r B u i l d i n g = p P r a e n t - > m _ s t r C u r M a i n B u i l d i n g N a m e ;  
         s t r S u b B u i l d i n g = p P r a e n t - > m _ s t r C u r S u b B u l d i n g N a m e ;  
         C S t r i n g   s t r I P , s t r P o r t ;  
         s t r P o r t . F o r m a t ( _ T ( " % d " ) , p r o d u c t _ r e g i s t e r _ v a l u e [ 7 5 ] ) ;  
         / /   B Y T E   a d d r e s s 1 , a d d r e s s 2 , a d d r e s s 3 , a d d r e s s 4 ;  
         m _ i p a d d r e s s . G e t A d d r e s s ( a d d r e s s 1 , a d d r e s s 2 , a d d r e s s 3 , a d d r e s s 4 ) ;  
         s t r I P . F o r m a t ( _ T ( " % d . % d . % d . % d " ) , a d d r e s s 1 , a d d r e s s 2 , a d d r e s s 3 , a d d r e s s 4 ) ;  
         i f ( g _ C o m m u n i c a t i o n T y p e = = 1 ) / / T C P  
         {  
               / *   i n t   n P o r t ;  
                 n P o r t = _ w t o i ( s t r P o r t ) ; * /  
                 / / m _ s t r I P = b u i l d _ i n f o . s t r I p ;    
                 g _ C o m m u n i c a t i o n T y p e = 1 ;  
                 S e t C o m m u n i c a t i o n T y p e ( g _ C o m m u n i c a t i o n T y p e ) ;  
                 B O O L   b O k = F A L S E ;  
                 b O k = O p e n _ S o c k e t 2 ( s t r I P , m _ n L i s t e n P o r t ) ;  
                 C S t r i n g   s t r I n f o ;  
  
                 s t r I n f o . F o r m a t ( ( _ T ( " O p e n   I P : % s   s u c c e s s f u l " ) ) , s t r I P ) ; / / p r o m p t   i n f o ;  
                 S e t P a n e S t r i n g ( 1 , s t r I n f o ) ;  
                 i f ( b O k )  
                 {  
                         t r y  
                         {  
                                 C B A D O   b a d o ;  
                                 b a d o . S e t D B P a t h ( g _ s t r C u r B u i l d i n g D a t a b a s e f i l e P a t h ) ;  
                                 b a d o . O n I n i t A D O C o n n ( ) ;    
  
                                 C S t r i n g   s t r S q l ;  
                                 / / s t r S q l . F o r m a t ( _ T ( " u p d a t e   B u i l d i n g   s e t   I p _ A d d r e s s = ' % s '   w h e r e   I p _ A d d r e s s = ' % s ' " ) , s t r I P , p P r a e n t - > m _ s t r I P ) ;  
                                 / / t _ p C o n - > E x e c u t e ( s t r S q l . G e t S t r i n g ( ) , N U L L , a d C m d T e x t ) ;  
  
                                 / /   � � n o d e  
                                 C S t r i n g   s t r S I D ;  
  
                                 s t r S I D . F o r m a t ( _ T ( " % d " ) ,   g e t _ s e r i a l n u m b e r ( ) ) ;  
                                 s t r S q l . F o r m a t ( _ T ( " u p d a t e   A L L _ N O D E   s e t   B a u t r a t e = ' % s ' , C o m _ P o r t = ' % s '   w h e r e   S e r i a l _ I D = ' % s ' " ) , s t r I P , s t r P o r t , s t r S I D ) ;   / / b a u t r a t e   � � I P  
                                 b a d o . m _ p C o n n e c t i o n - > E x e c u t e ( s t r S q l . G e t S t r i n g ( ) , N U L L , a d C m d T e x t ) ;  
  
                                 b a d o . C l o s e C o n n ( ) ;  
  
                         }  
                         c a t c h ( _ c o m _ e r r o r   * e )  
                         {  
                                 A f x M e s s a g e B o x ( e - > E r r o r M e s s a g e ( ) ) ;  
                         }  
  
                         p P r a e n t - > m _ s t r I P = s t r I P ;  
                         p P r a e n t - > S c a n T s t a t I n D B ( ) ;  
  
                 }  
                 e l s e  
                 {  
  
                 }  
  
  
  
         }  
         E n d W a i t C u r s o r ( ) ;  
 }  
  
  
 v o i d   T 3 0 0 0 _ D e f a u l t _ M a i n V i e w : : O n C b n S e l c h a n g e I p m o d e l C o m b o ( )  
 {        
 }  
 