#pragma once
#include <QPushButton>

class SudokuCell : public QPushButton {
  Q_OBJECT
public:
  enum HighlightState {
    Default,
    Selected,
    Related, // Для связанных ячеек (строка/колонка/блок)
    SameDigit // Для одинаковых цифр
  };

  SudokuCell(int row, int col, QWidget *parent = nullptr);
  void setDisplayValue(int value);
  void setOriginal(bool isOriginal);
  bool isOriginal() const { return isOriginal_; }
  int GetRow() const { return row; }
  int GetCol() const { return col; }

  void setNote(int value, bool on);
  bool hasNote(int value) const;
  void clearNotes();

  // Методы для управления подсветкой
  void setHighlightState(HighlightState state);
  HighlightState getHighlightState() const { return highlightState; }

signals:
  // Сигнал клика на ячейку
  void cellClicked(int row, int col);
  void cellDoubleClicked(int row, int col);

protected:
  void paintEvent(QPaintEvent *event) override; // Добавляем объявление
  void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
  int row;
  int col;
  int currentValue_ = 0; // Текущее значение ячейки
  bool isOriginal_ = false;
  HighlightState highlightState = Default; // Состояние подсветки
  std::array<bool, 9> notes_;              // Пометки в ячейке
  void updateTextColor(); // Обновление цвета текста
};
