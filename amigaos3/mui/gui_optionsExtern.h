#define reg(a) __asm(#a)

void Reload(struct Hook *h, Object *o);
void StartScreen(struct Hook *h, Object *o);
void StopScreen(struct Hook *h, Object *o);
void Restart(struct Hook *h, Object *o);
void Cancel(struct Hook *h, Object *o);
void Save(struct Hook *h, Object *o);
void SetUrl(struct Hook *h, Object *o);
void GetFavAttr(struct Hook *h, Object *o);
struct Ulubione *ulubione;
long FavConstructor (struct Ulubione *ulubione reg(a1), APTR mempool reg(a2));
long FavDestructor (struct Ulubione *ulubione reg(a1), APTR mempool reg(a2));
long FavDisplayer (struct Ulubione *ulubione reg(a1), char **teksty reg(a2));
void ModifyFav(struct Hook *h, Object *o);
void Proxy(struct Hook *h, Object *o);

