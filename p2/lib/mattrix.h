void Add(double **a, double **b);
void Mul(double **a, double **b, double **res);
void Muld(double **a, double x);
double Det(double **a, int len);
void Print(double **a, int len);
void Input(double **a, int len);
void SetMinor(double **a, double **p, int ci, int cj, int len);
void Delete(double **a, int len);
double ** Create(double len);
void Copy(double ** to, double ** from);