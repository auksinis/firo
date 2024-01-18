#ifndef FIRO_QT_AUTOMINT_H
#define FIRO_QT_AUTOMINT_H

#include "walletmodel.h"

#include <QDialog>
#include <QPainter>
#include <QPaintEvent>

enum class AutoMintMode : uint8_t {
    MintAll, // come from overview page
    AutoMintAll // come from notification
};

namespace Ui {
    class AutoMintDialog;
}

class AutoMintDialog : public QDialog
{
    Q_OBJECT;

public:
    explicit AutoMintDialog(AutoMintMode mode, QWidget *parent = 0);
    ~AutoMintDialog();

public:
    int exec();
    void setModel(WalletModel *model);

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void accept();
    void reject();

private:
    enum class AutoMintProgress : uint8_t {
        Start,
        Unlocking,
        Minting
    };

    Ui::AutoMintDialog *ui;
    WalletModel *model;
    LelantusModel *lelantusModel;
    bool requiredPassphase;
    AutoMintProgress progress;
    AutoMintMode mode;

    void ensureLelantusModel();
};

enum class AutoMintSparkMode : uint8_t {
    MintAll, // come from overview page
    AutoMintAll // come from notification
};

class AutoMintSparkDialog : public QDialog
{
    Q_OBJECT;

public:
    explicit AutoMintSparkDialog(AutoMintSparkMode mode, QWidget *parent = 0);
    ~AutoMintSparkDialog();

public:
    int exec();
    void setModel(WalletModel *model);

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void accept();
    void reject();

private:
    enum class AutoMintSparkProgress : uint8_t {
        Start,
        Unlocking,
        Minting
    };

    Ui::AutoMintDialog *ui;
    WalletModel *model;
    SparkModel *sparkModel;
    bool requiredPassphase;
    AutoMintSparkProgress progress;
    AutoMintSparkMode mode;

    void ensureSparkModel();
};

#endif // FIRO_QT_AUTOMINT_H