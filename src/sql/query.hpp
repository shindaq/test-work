#pragma once

namespace sql {
constexpr static const char* kCreateUser =
    R"~(
        INSERT INTO
            stock_schema.users(name)
        VALUES 
            ($1)
        RETURNING 
            id
    )~";

constexpr static const char* kSelectUser =
    R"~(
        SELECT 
            id 
        FROM 
            stock_schema.users
        WHERE
            name = $1
    )~";

constexpr static const char* kInsertRequisition =
    R"~(
        INSERT INTO
            stock_schema.requisitions(user_id, volume, exchange_rate, operation_type)
        VALUES
            ($1, $2, $3, $4)
        RETURNING 
            id
    )~";

constexpr static const char* kSelectSellRequisitions =
    R"~(
        SELECT id, user_id, volume, exchange_rate FROM 
            stock_schema.requisitions 
        WHERE 
            user_id <> $1 AND operation_type = 'sell' AND status = 'open' 
        ORDER BY exchange_rate ASC, created_at ASC
        LIMIT 1
    )~";

constexpr static const char* kSelectBuyRequisitions =
    R"~(
        SELECT id, user_id, volume, exchange_rate FROM 
            stock_schema.requisitions 
        WHERE 
            user_id <> $1 AND operation_type = 'buy' AND status = 'open' 
        ORDER BY 
            exchange_rate DESC, created_at ASC 
        LIMIT 1
    )~";

constexpr static const char* kUpdateRequisition =
    R"~(
        UPDATE
            stock_schema.requisitions
        SET
            volume = volume + $2
        WHERE
            id = $1 AND volume + $2 <> 0 
    )~";

constexpr static const char* kUpdateRequisitionStatus =
    R"~(
        UPDATE
            stock_schema.requisitions
        SET
            status = 'closed'
        WHERE
            id = $1
    )~";

constexpr static const char* kUpdateUserBalance =
    R"~(
        UPDATE
            stock_schema.users
        SET
            usd_balance = usd_balance + $2, rub_balance = rub_balance + $3
        WHERE
            id = $1
    )~";

constexpr static const char* kTakeUser =
    R"~(
        SELECT
            id, name, usd_balance, rub_balance
        FROM
            stock_schema.users
        WHERE
            id = $1
    )~";

constexpr static const char* kTakeActiveRequisitions =
    R"~(
        SELECT
            id, created_at, user_id, volume, exchange_rate, operation_type
        FROM
            stock_schema.requisitions
        WHERE
            status = 'open'
    )~";

constexpr static const char* kInsertDeal = 
    R"~(
        INSERT INTO
            stock_schema.deals(buyer_id, seller_id, volume, exchange_rate)
        VALUES
            ($1,$2,$3,$4)
    )~";

constexpr static const char* kTakeDeals = 
    R"~(
        SELECT
            *
        FROM
            stock_schema.deals
        WHERE 
            buyer_id = $1 OR seller_id = $1
        ORDER BY
            created_at DESC
    )~";
}  // namespace sql