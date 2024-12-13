Instructions de compilation et exécution :

  make/make all : pour une compilation globale
  make test : tests unitaires
  make perf : mesure de la performance de ma fonction malloc de base
  make perf_2.0 : mesure de la performance de ma fonction malloc avec améliorations
  make clean : élimine les fichiers objets

Le projet se découpe en plusieurs fichiers, il y a au total deux allocateurs personnalisés. Un très basique, utilisant seulement mmap et munmap, que j'ai testé dans le fichier test.c, puis une version optimisée.
Pour chacun il y a un fichier .h et .c, et enfin il y a deux fichiers perf.c (resp. perf_2.0.c) qui servent aux mesures de performances des allocateurs mem_alloc.c (resp. mem_alloc_2.0.c).

Ce qu'il y a de commun :

  - La structure Block qui contient une taille de bloc ainsi qu'un pointeur vers le bloc suivant (utile pour les listes chaînées).
  - L'utilisation de mmap et munmap qui sont responsables d'une allocation et libération "grossière", que l'on affinera dans l'allocateur optimisé.

L'optimisation

  Cache par thread : L'idée est d'utiliser des caches locaux par thread pour réduire les coûts d'accès à la mémoire partagée et améliorer la performance. Chaque thread possède donc son propre cache (struct ThreadCache), 
qui contient des listes chaînées de blocs pour des tailles variées. De cette façon, on minimise les appels à mmap et munmap en réutilisant des blocs déjà alloués. Le cache local est implémenté avec "pthread_key_t" 
qui permet de lier un cache spécifique à chaque thread. 
Mmap est donc utilisé uniquement lorsque le cache local est insuffisant, et lorsque de la mémoire est libérée, le bloc est renvoyé dans le cache local du thread.

  Segmentation en classes de tailles, et implémentation de la fusion de blocs : Dans une volonté d'affiner l'allocation de la mémoire, le code va fragmenter cette dernière afin d'allouer des quantités plus proches
de ce que demande l'utilisateur. Mais une fragmentation trop fréquente peut mener à des erreurs, il fallait donc implémenter la fusion de blocs libres. C'est la fonction my_free_2_0 qui s'en charge en effectuant une
vérification lors de la libération, à savoir : "Est-ce qu'il y a deux blocs mémoires, libres et contigus dans le cache ?" Si la réponse est oui, il y a une fusion.

Une fois ces améliorations apportées, j'ai pu observer un impact sur les performances. J'ai donc construit un graphique qui comparait non seulement le mem_alloc et mem_alloc_2.0, mais qui comportait aussi les
performances des fonctions "malloc/free". 

![image](https://github.com/user-attachments/assets/2e7cfcc2-a238-4ff3-b494-7588a1d62324)
