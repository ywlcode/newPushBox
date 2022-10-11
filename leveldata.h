#ifndef LEVELDATA_H
#define LEVELDATA_H
#define MAXN 15
#include <QObject>

class LevelData : public QObject
{
	Q_OBJECT
public:
	explicit LevelData(int l, QObject* parent = nullptr);
	int m[MAXN][MAXN];
};

#endif // LEVELDATA_H
